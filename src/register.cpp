#include "register.hpp"

namespace stacksafe {

Register::Register(const llvm::Value* v) : val_{v} {}

}  // namespace stacksafe
