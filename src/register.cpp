#include "register.hpp"

namespace stacksafe {

bool operator<(const Register &lhs, const Register &rhs) {
  return lhs.value() < rhs.value();
}
std::string to_str(const Register &reg) {
  static const std::string prefix{"%"};
  return prefix + std::to_string(reg.value());
}

}  // namespace stacksafe
