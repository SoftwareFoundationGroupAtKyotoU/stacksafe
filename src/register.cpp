#include "register.hpp"
#include "utility.hpp"

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
std::string to_str(const Register &reg) {
  static const std::string prefix{"&"};
  static const std::string global{"@"};
  if (reg.is_local()) {
    return prefix + std::to_string(register_number(*reg.value()).value_or(-1));
  } else {
    return prefix + global;
  }
}

}  // namespace stacksafe
