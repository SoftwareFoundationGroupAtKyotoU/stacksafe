#include "stacksafe.hpp"

namespace stacksafe {

char StackSafe::ID = 0;
StackSafe::StackSafe() : llvm::ModulePass{ID} {}

}  // namespace stacksafe
