#include "value.hpp"
#include <llvm/IR/Argument.h>
#include <llvm/IR/Instruction.h>
#include "hash.hpp"

namespace stacksafe {

Value::Value(const Base &base) : Base{base}, ptr_{nullptr} {}
Value::Value(const llvm::Value &val) : Base{val}, ptr_{nullptr} {}
Value::Value(const void *sym, bool is_local)
    : Base{sym, is_local}, ptr_{nullptr} {}
const llvm::Value *Value::value() const {
  return static_cast<const llvm::Value *>(Base::reg());
}
bool Value::is_global() const {
  return Base::is_global();
}
bool Value::is_local() const {
  return Base::is_local();
}
bool Value::is_argument() const {
  return !is_local() && !is_global();
}
Value Value::get_symbol() {
  return Value{nullptr, false};
}
Value Value::get_symbol(const llvm::AllocaInst &v) {
  return Value{&v, true};
}
Value Value::get_symbol(const llvm::Argument &v) {
  return Value{&v, false};
}
Value Value::get_register(const llvm::Argument &v) {
  return Value{v};
}
Value Value::get_register(const llvm::Instruction &v) {
  return Value{v};
}
bool Value::operator==(const Value &sym) const {
  return Base::equals(*this, sym);
}
bool Value::operator<(const Value &sym) const {
  return Base::less(*this, sym);
}

}  // namespace stacksafe
