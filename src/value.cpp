#include "value.hpp"
#include "utility.hpp"

namespace stacksafe {

Value::Value(const Symbol &sym) : sym_{sym} {}
Value::Value(const Register &reg) : reg_{reg} {}
const llvm::Value *Value::value() const {
  if (is_symbol()) {
    return as_symbol()->value();
  } else {
    return &as_register()->value();
  }
}
const void *Value::ptr() const {
  return ptr_;
}
bool Value::is_symbol() const {
  return least_significant_bit(ptr_);
}
const Symbol *Value::as_symbol() const {
  if (is_symbol()) {
    return &sym_;
  } else {
    return nullptr;
  }
}
const Register *Value::as_register() const {
  if (is_symbol()) {
    return nullptr;
  } else {
    return &reg_;
  }
}
Value Value::get_symbol() {
  return Value{Symbol::get_global()};
}
Value Value::get_symbol(const llvm::AllocaInst &v) {
  return Value{Symbol{v}};
}
Value Value::get_symbol(const llvm::Argument &v) {
  return Value{Symbol{v}};
}
Value Value::get_register(const llvm::Argument &v) {
  return Value{Register{v}};
}
Value Value::get_register(const llvm::Instruction &v) {
  return Value{Register{v}};
}
bool operator==(const Value &lhs, const Value &rhs) {
  return lhs.ptr() == rhs.ptr();
}
bool operator<(const Value &lhs, const Value &rhs) {
  return lhs.ptr() < rhs.ptr();
}

}  // namespace stacksafe
