#ifndef INCLUDE_GUARD_24D0B7C2_7662_4CA7_9DAE_CFC9EA3B0EA0
#define INCLUDE_GUARD_24D0B7C2_7662_4CA7_9DAE_CFC9EA3B0EA0

#include "llvm/Pass.h"

namespace llvm {
  class Function;
}

namespace stacksafe {
  class Analyzer : public llvm::FunctionPass {
  public:
    Analyzer();
    static char ID;
    bool runOnFunction(llvm::Function &F) override;
  };
}

#endif//INCLUDE_GUARD_24D0B7C2_7662_4CA7_9DAE_CFC9EA3B0EA0
