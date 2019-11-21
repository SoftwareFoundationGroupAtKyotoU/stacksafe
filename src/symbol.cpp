#include "symbol.hpp"
#include "hash.hpp"

namespace stacksafe {

Symbol::Symbol(const llvm::Value &val) : key_{val} {}
Symbol::Symbol(const void *sym, bool is_arg) : key_{sym, is_arg} {}
const llvm::Value *Symbol::value() const {
  return key_.value();
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
  return Symbol{nullptr, false};
}
Symbol Symbol::get_local(const llvm::AllocaInst &v) {
  return Symbol{&v, false};
}
Symbol Symbol::get_arg(const llvm::Argument &v) {
  return Symbol{&v, true};
}
Symbol Symbol::get_register(const llvm::Value &v) {
  return Symbol{v};
}
bool Symbol::operator==(const Symbol &sym) const {
  return Key::equals(key_, sym.key_);
}
bool Symbol::operator<(const Symbol &sym) const {
  return Key::less(key_, sym.key_);
}

}  // namespace stacksafe

namespace std {
size_t hash<stacksafe::Symbol>::operator()(const stacksafe::Symbol &sym) const {
  return hash_value(sym);
}
}  // namespace std
