#include "value.hpp"
#include <llvm/IR/Value.h>
#include "fabric.hpp"
#include "json.hpp"
#include "utility.hpp"

namespace stacksafe {

const std::string Value::prefix_{"%"};
Value::Value(int n, const llvm::Value &v, Kind k)
    : Token{n, Type{v.getType()}}, value_{&v}, kind_{k} {}
Value::Value(int n, const llvm::Value &v)
    : Token{n, Type{v.getType()}}, value_{&v}, kind_{Kind::REGISTER} {}
Value::Value(const llvm::Value &v, Kind k)
    : Token{-1, Type{v.getType()}}, value_{&v}, kind_{k} {}
Value Value::make(const llvm::Value &v) {
  if (check_register(v)) {
    auto operand = get_operand(v);
    std::string_view view{operand};
    if (!view.empty() && view.substr(0, 1) == prefix_) {
      if (auto i = to_int(view.substr(1))) {
        return Value{*i, v, Kind::REGISTER};
      }
    }
    stacksafe_unreachable("failed register check", v);
  } else if (check_constant(v)) {
    return Value{-1, v, Kind::CONSTANT};
  } else {
    stacksafe_unreachable("neither register nor constant", v);
  }
}
Value::Value(int n)
    : Token{n, Type{nullptr}}, value_{nullptr}, kind_{Kind::REGISTER} {}
const llvm::Value *Value::get() const { return value_; }
std::string Value::repr() const {
  switch (kind_) {
  case Kind::REGISTER:
    return prefix_ + std::to_string(number());
  case Kind::CONSTANT:
    [[fallthrough]];
  default:
    return to_str(*value_);
  }
  stacksafe_unreachable("unknown kind", *this);
}
int Value::kind() const { return static_cast<int>(kind_); }
bool Value::is_register() const { return kind_ == Kind::REGISTER; }
bool operator<(const Value &lhs, const Value &rhs) {
  if (lhs.is_register() && rhs.is_register()) {
    return lhs.number() < rhs.number();
  } else if (lhs.kind() == rhs.kind()) {
    return lhs.repr() < rhs.repr();
  } else {
    return lhs.kind() < rhs.kind();
  }
}
void to_json(Json &j, const Value &x) { j = x.repr() + x.type().repr(); }
Fabric dump(const Value &value) {
  Fabric ret;
  return ret.append(value.repr()).quote();
}

}  // namespace stacksafe
