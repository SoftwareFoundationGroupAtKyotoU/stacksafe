#include "register.hpp"

namespace stacksafe {

Register::Register(int n) : Number{n} {}
const Number &Register::number() const {
  return *this;
}
Register Register::make(int n) {
  return Register{n};
}
std::string to_str(const Register &reg) {
  static const std::string prefix{"%"};
  return prefix + to_str(reg.number());
}

}  // namespace stacksafe
