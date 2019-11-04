#include "register.hpp"

namespace stacksafe {

Register::Register(int n) : Number{n} {}
const Number &Register::number() const {
  return *this;
}
const Register &Register::get_global() {
  static Register global{-1};
  return global;
}
std::string to_str(const Register &reg) {
  static const std::string prefix{"%"};
  return prefix + to_str(reg.number());
}

}  // namespace stacksafe
