#include "value.hpp"
#include <llvm/ADT/StringRef.h>
#include <llvm/IR/Value.h>
#include "domain.hpp"
#include "fabric.hpp"
#include "json.hpp"
#include "symbol.hpp"
#include "utility.hpp"

namespace stacksafe {

const std::string Value::prefix_{"%"};
Value::Value(int n, const llvm::Value &v, Kind k)
    : Token{n, Type{v.getType()}}, value_{&v}, kind_{k} {}
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
  } else if (check_global(v)) {
    return Value{-1, v, Kind::GLOBAL};
  } else {
    assert(check_constant(v) && "neither register nor constant");
    return Value{-1, v, Kind::CONSTANT};
  }
}
Value::Value(int n)
    : Token{n, Type{nullptr}}, value_{nullptr}, kind_{Kind::REGISTER} {}
const llvm::Value *Value::get() const { return value_; }
std::string Value::repr() const {
  switch (kind_) {
  default:
  case Kind::REGISTER:
    return prefix_ + std::to_string(number());
  case Kind::CONSTANT:
    return to_str(*value_);
  case Kind::GLOBAL:
    return "@" + value_->getName().str() + type().repr();
  }
}
bool Value::is_register() const { return kind_ == Kind::REGISTER; }
Domain Value::get_domain() const {
  switch (kind_) {
  case Kind::CONSTANT:
    return Domain{};
  case Kind::GLOBAL:
    return Domain{Symbol::global()};
  default:
    stacksafe_unreachable("get_domain must be called from non-register", *this);
  }
}
bool operator<(const Value &lhs, const Value &rhs) {
  assert(lhs.is_register() && rhs.is_register() &&
         "value comparison must be on registers");
  return lhs.number() < rhs.number();
}
void to_json(Json &j, const Value &x) { j = x.repr() + x.type().repr(); }
Fabric dump(const Value &value) {
  Fabric ret;
  return ret.append(value.repr()).quote();
}

}  // namespace stacksafe
