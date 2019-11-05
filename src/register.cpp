#include "register.hpp"

namespace stacksafe {

Symbol::Symbol(const llvm::Value *v) : reg_{v} {}
const llvm::Value *Symbol::value() const {
  return reg_;
}
bool Symbol::is_local() const {
  return reg_ != nullptr;
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
size_t hash<stacksafe::Symbol>::operator()(const stacksafe::Symbol &reg) const {
  return hash<const llvm::Value *>{}(reg.value());
}
}  // namespace std
