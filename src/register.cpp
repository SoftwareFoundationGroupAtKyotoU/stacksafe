#include "register.hpp"
#include "fabric.hpp"

namespace stacksafe {

const std::string Register::prefix{"%"};
Register::Register(int n) : num_{n} {}
int Register::number() const {
  return num_;
}
std::string Register::repr() const {
  return prefix + std::to_string(number());
}
bool operator<(const Register &lhs, const Register &rhs) {
  return lhs.number() < rhs.number();
}
std::string to_str(const Register &reg) {
  static const std::string prefix{"%"};
  return prefix + std::to_string(reg.number());
}
Fabric dump(const Register &reg) {
  Fabric ret;
  return ret.append(reg.repr()).quote();
}

}  // namespace stacksafe
