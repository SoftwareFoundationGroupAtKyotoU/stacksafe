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
void StackSafe::print(llvm::raw_ostream&, const llvm::Module*) const {}

static llvm::RegisterPass<StackSafe> registerpass(stacksafe, stacksafe, false,
                                                  true);

}  // namespace stacksafe
