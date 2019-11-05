#include "register.hpp"

namespace stacksafe {

Register::Register(const llvm::Value *v) : reg_{v} {}
const llvm::Value *Register::value() const {
  return reg_;
}
bool Register::is_local() const {
  return reg_ != nullptr;
}
const Register &Register::get_global() {
  static Register global{nullptr};
  return global;
}
Register Register::get_local(const llvm::Value &v) {
  return Register{&v};
}
bool operator<(const Register &lhs, const Register &rhs) {
  return lhs.value() < rhs.value();
}

}  // namespace stacksafe
