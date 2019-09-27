#include "register.hpp"

namespace stacksafe {

Register::Register(int n) : num_{n} {}
int Register::number() const { return num_; }

}  // namespace stacksafe
