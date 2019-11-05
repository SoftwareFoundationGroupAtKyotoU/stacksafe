#include "symbol.hpp"

namespace stacksafe {

Symbol::Symbol(const llvm::Value *v) : Value{v} {}
bool Symbol::is_local() const {
  return value() != nullptr;
}
const Symbol &Symbol::get_global() {
  static Symbol global{nullptr};
  return global;
}
Symbol Symbol::get_local(const llvm::Value &v) {
  return Symbol{&v};
}
bool operator<(const Symbol &lhs, const Symbol &rhs) {
  return lhs.value() < rhs.value();
}
bool operator==(const Symbol &lhs, const Symbol &rhs) {
  return lhs.value() == rhs.value();
}

}  // namespace stacksafe

namespace std {
size_t hash<stacksafe::Symbol>::operator()(const stacksafe::Symbol &sym) const {
  return hash<const llvm::Value *>{}(sym.value());
}
}  // namespace std
