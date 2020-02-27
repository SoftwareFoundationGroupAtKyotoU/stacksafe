#include "stacksafe.hpp"

namespace stacksafe {
namespace {
constexpr auto stacksafe{"stacksafe"};
}

char StackSafe::ID = 0;
StackSafe::StackSafe() : llvm::ModulePass{ID} {}
bool StackSafe::runOnModule(llvm::Module&) {
  return false;
}

static llvm::RegisterPass<StackSafe> registerpass(stacksafe, stacksafe, false,
                                                  true);

}  // namespace stacksafe
