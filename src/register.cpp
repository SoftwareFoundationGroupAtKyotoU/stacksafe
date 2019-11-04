#include "register.hpp"

namespace stacksafe {

Register::Register(int n) : Symbol{n}, num_{n} {}
int Register::number() const {
  return num_;
}
bool Register::is_local() const {
  return get_global() < *this;
}
const Register &Register::get_global() {
  static Register global{-1};
  return global;
}
bool operator<(const Register &lhs, const Register &rhs) {
  return lhs.number() < rhs.number();
}
std::string to_str(const Register &reg) {
  static const std::string prefix{"%"};
  return prefix + std::to_string(reg.number());
}

}  // namespace stacksafe
