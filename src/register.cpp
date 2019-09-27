#include "register.hpp"

namespace stacksafe {

const std::string Register::prefix{"%"};
Register::Register(int n) : num_{n} {}
int Register::number() const { return num_; }
std::string Register::repr() const { return prefix + std::to_string(number()); }

}  // namespace stacksafe
