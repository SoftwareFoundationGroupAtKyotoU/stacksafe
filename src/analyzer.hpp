#ifndef INCLUDE_GUARD_24D0B7C2_7662_4CA7_9DAE_CFC9EA3B0EA0
#define INCLUDE_GUARD_24D0B7C2_7662_4CA7_9DAE_CFC9EA3B0EA0

#include "abstraction.hpp"
#include <llvm/Pass.h>
#include <memory>

namespace llvm {
class BasicBlock;
class Function;
class TerminatorInst;
} // namespace llvm

namespace stacksafe {
class Analyzer : public llvm::FunctionPass {
  std::unique_ptr<Abstraction> abst_;

public:
  static char ID;

public:
  Analyzer();
  void print(llvm::raw_ostream &O, const llvm::Module *M) const override;
  bool runOnFunction(llvm::Function &F) override;
};
} // namespace stacksafe

#endif // INCLUDE_GUARD_24D0B7C2_7662_4CA7_9DAE_CFC9EA3B0EA0
