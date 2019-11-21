#include "value.hpp"
#include <llvm/IR/Argument.h>
#include <llvm/IR/Instruction.h>
#include "hash.hpp"

namespace stacksafe {

Value::Value(const llvm::Value &val) : key_{val} {}
Value::Value(const void *sym, bool is_arg) : key_{sym, is_arg} {}
const llvm::Value *Value::value() const {
  return key_.value();
}
bool Value::is_global() const {
  return key_.is_global();
}
bool Value::is_local() const {
  return key_.is_local();
}
bool Value::is_argument() const {
  return !is_local() && !is_global();
}
Value Value::get_symbol() {
  return Value{nullptr, false};
}
Value Value::get_symbol(const llvm::AllocaInst &v) {
  return Value{&v, false};
}
Value Value::get_symbol(const llvm::Argument &v) {
  return Value{&v, true};
}
Value Value::get_register(const llvm::Argument &v) {
  return Value{v};
}
Value Value::get_register(const llvm::Instruction &v) {
  return Value{v};
}
bool Value::operator==(const Value &sym) const {
  return Base::equals(key_, sym.key_);
}
bool Value::operator<(const Value &sym) const {
  return Base::less(key_, sym.key_);
}

}  // namespace stacksafe
