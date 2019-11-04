#include "register.hpp"

namespace stacksafe {

bool operator<(const Register &lhs, const Register &rhs) {
  return lhs.number() < rhs.number();
}
std::string to_str(const Register &reg) {
  static const std::string prefix{"%"};
  return prefix + std::to_string(reg.number());
}

}  // namespace stacksafe
