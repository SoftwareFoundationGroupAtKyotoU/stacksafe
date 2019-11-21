#include "symbol.hpp"
#include <llvm/IR/Instructions.h>
#include "utility.hpp"
#include "value.hpp"

namespace stacksafe {

Symbol::Symbol() : key_{} {}
Symbol::Symbol(const llvm::AllocaInst &val) : key_{val} {}
Symbol::Symbol(const llvm::Argument &val) : key_{val} {}
Symbol::Symbol(const llvm::Instruction &val) : key_{val} {}
Symbol::Symbol(const void *sym, bool is_arg) : key_{sym, is_arg} {}
Value Symbol::value() const {
  const auto ptr = key_.value();
  return ptr ? Value{*ptr} : Value{};
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
  return Symbol{&v, false};
}
Symbol Symbol::get_arg(const llvm::Argument &v) {
  return Symbol{&v, true};
}
Symbol Symbol::get_register(const llvm::Instruction &v) {
  return Symbol{v};
}
std::size_t Symbol::hash(const Symbol &sym) {
  return Value::hash(sym.value());
}
bool Symbol::operator==(const Symbol &sym) const {
  return Key::equals(key_, sym.key_);
}
bool Symbol::operator<(const Symbol &sym) const {
  return Key::less(key_, sym.key_);
}
llvm::hash_code hash_value(const Symbol &sym) {
  return hash_value(sym.key_);
}

}  // namespace stacksafe
