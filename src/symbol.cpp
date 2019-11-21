#include "symbol.hpp"
#include <llvm/IR/Instructions.h>
#include "utility.hpp"

namespace stacksafe {

Symbol::Symbol() : Value{}, key_{} {}
Symbol::Symbol(const llvm::AllocaInst &val) : Value{val}, key_{val} {}
Symbol::Symbol(const llvm::Argument &val) : Value{val}, key_{val} {}
Symbol::Symbol(const llvm::Instruction &val) : Value{val}, key_{val} {}
const Value &Symbol::value() const {
  return *this;
}
bool Symbol::is_global() const {
  return key_.is_global();
}
bool Symbol::is_local() const {
  return key_.is_local();
}
bool Symbol::is_argument() const {
  return key_.is_argument();
}
Symbol Symbol::get_global() {
  return Symbol{};
}
Symbol Symbol::get_local(const llvm::AllocaInst &v) {
  return Symbol{v};
}
Symbol Symbol::get_arg(const llvm::Argument &v) {
  return Symbol{v};
}
Symbol Symbol::get_register(const llvm::Instruction &v) {
  return Symbol{v};
}
std::size_t Symbol::hash(const Symbol &sym) {
  return Value::hash(sym.value());
}
bool operator==(const Symbol &lhs, const Symbol &rhs) {
  return lhs.value() == rhs.value();
}
bool operator<(const Symbol &lhs, const Symbol &rhs) {
  return lhs.value() < rhs.value();
}
llvm::hash_code hash_value(const Symbol &sym) {
  return hash_value(sym.key_);
}

}  // namespace stacksafe
