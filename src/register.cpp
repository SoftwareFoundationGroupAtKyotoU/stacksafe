#include "register.hpp"

namespace stacksafe {

const Number &Register::number() const {
  return *this;
}
std::string to_str(const Register &reg) {
  static const std::string prefix{"%"};
  return prefix + to_str(reg.number());
}

}  // namespace stacksafe
