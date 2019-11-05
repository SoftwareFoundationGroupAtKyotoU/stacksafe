#include "register.hpp"

namespace stacksafe {

Register::Register(const llvm::Value& v) : Value{&v} {}

}  // namespace stacksafe
