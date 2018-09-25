#ifndef INCLUDE_GUARD_24D0B7C2_7662_4CA7_9DAE_CFC9EA3B0EA0
#define INCLUDE_GUARD_24D0B7C2_7662_4CA7_9DAE_CFC9EA3B0EA0

#include "environment.hpp"
#include <queue>
#include <unordered_map>
#include <vector>
#include <llvm/Pass.h>

namespace llvm {
  class BasicBlock;
  class Function;
}

namespace stacksafe {
  class Analyzer : public llvm::FunctionPass {
  public:
    Analyzer();
    static char ID;
    bool runOnFunction(llvm::Function &F) override;
  };
  using Successors = std::vector<llvm::BasicBlock *>;
  Successors getSuccessors(llvm::BasicBlock &B);

  class State {
    std::unordered_map<llvm::BasicBlock *, Environment> map_;
    std::queue<llvm::BasicBlock *> todo_;
  private:
    void init(llvm::Function &F);
    void traverse();
  };
}

#endif//INCLUDE_GUARD_24D0B7C2_7662_4CA7_9DAE_CFC9EA3B0EA0
