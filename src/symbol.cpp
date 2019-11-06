#include "symbol.hpp"
#include <llvm/IR/Instructions.h>

namespace stacksafe {

Symbol::Symbol(const Value &v) : Value{v} {}
const Value &Symbol::value() const {
  return *this;
}
bool Symbol::is_local() const {
  return static_cast<bool>(value());
}
const Symbol &Symbol::get_global() {
  static Symbol global{Value{}};
  return global;
}
Symbol Symbol::get_local(const llvm::AllocaInst &v) {
  return Symbol{v};
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
  return sym.hash();
}
}  // namespace std
