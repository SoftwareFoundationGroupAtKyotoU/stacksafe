#include "value.hpp"
#include <llvm/IR/Value.h>
#include "fabric.hpp"
#include "json.hpp"
#include "utility.hpp"

namespace stacksafe {

const std::string Value::prefix_{"%"};
Value::Value(int n, const llvm::Value &v)
    : Token{n, Type{v.getType()}}, kind_{Kind::REGISTER}, value_{&v} {}
Value::Value(const llvm::Value &v)
    : Token{-1, Type{v.getType()}}, kind_{Kind::CONSTANT}, value_{&v} {}
Value Value::make(const llvm::Value &v) {
  if (check_register(v)) {
    auto operand = get_operand(v);
    std::string_view view{operand};
    if (!view.empty() && view.substr(0, 1) == prefix_) {
      if (auto i = to_int(view.substr(1))) {
        return Value{*i, v};
      }
    }
    stacksafe_unreachable("failed register check", v);
  } else if (check_constant(v)) {
    return Value{v};
  } else {
    stacksafe_unreachable("neither register nor constant", v);
  }
}
Value::Value(int n)
    : Token{n, Type{nullptr}}, kind_{Kind::OTHER}, value_{nullptr} {}
const llvm::Value *Value::get() const { return value_; }
std::string Value::repr() const {
  if (is_register()) {
    return prefix_ + std::to_string(number());
  } else {
    return to_str(*value_);
  }
}
bool Value::is_register() const { return 0 <= number(); }
bool operator<(const Value &lhs, const Value &rhs) {
  if (lhs.is_register() || rhs.is_register()) {
    return lhs.number() < rhs.number();
  } else {
    return lhs.repr() < rhs.repr();
  }
}
void to_json(Json &j, const Value &x) { j = x.repr() + x.type().repr(); }
Fabric dump(const Value &value) {
  Fabric ret;
  return ret.append(value.repr()).quote();
}

}  // namespace stacksafe
