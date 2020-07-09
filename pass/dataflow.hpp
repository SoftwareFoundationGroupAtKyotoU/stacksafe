#ifndef INCLUDE_GUARD_D8F6EC4F_4011_435A_AD3A_CF631A629063
#define INCLUDE_GUARD_D8F6EC4F_4011_435A_AD3A_CF631A629063

#include <llvm/Pass.h>

namespace dataflow {
class DataFlow : public ::llvm::ModulePass {
 public:
  static char ID;
  DataFlow();
  bool runOnModule(llvm::Module &m) override;
  void print(llvm::raw_ostream &os, const llvm::Module *) const override;
};
}  // namespace dataflow

#endif  // INCLUDE_GUARD_D8F6EC4F_4011_435A_AD3A_CF631A629063
