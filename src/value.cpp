#include "value.hpp"
#include <llvm/IR/Argument.h>
#include <llvm/IR/Instruction.h>
#include "hash.hpp"
#include "utility.hpp"

namespace stacksafe {

Value::Value(const Symbol &sym) : Base{sym.value(), true}, sym_{sym} {}
Value::Value(const Register &reg) : Base{reg.value()}, reg_{reg} {}
Value::Value(const Base &base) : Base{base}, ptr_{nullptr} {}
Value::Value(const llvm::Value &val) : Base{val}, ptr_{nullptr} {}
Value::Value(const void *sym, bool is_local)
    : Base{sym, is_local}, ptr_{nullptr} {}
const llvm::Value *Value::value() const {
  if (is_symbol()) {
    return sym_.value();
  } else {
    return &reg_.value();
  }
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
Value Value::get_symbol() {
  return Value{Symbol{}};
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
bool Value::operator==(const Value &sym) const {
  return Base::equals(*this, sym);
}
bool Value::operator<(const Value &sym) const {
  return Base::less(*this, sym);
}

}  // namespace stacksafe
