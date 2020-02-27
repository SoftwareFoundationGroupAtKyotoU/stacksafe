#ifndef INCLUDE_GUARD_4EF1A249_1905_4F41_BCD6_F1F1D615A441
#define INCLUDE_GUARD_4EF1A249_1905_4F41_BCD6_F1F1D615A441

#include <llvm/Pass.h>

namespace stacksafe {

class StackSafe : public ::llvm::ModulePass {
 public:
  static char ID;
  StackSafe();
  bool runOnModule(llvm::Module& m) override;
  void print(llvm::raw_ostream& os, const llvm::Module*) const override;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_4EF1A249_1905_4F41_BCD6_F1F1D615A441
