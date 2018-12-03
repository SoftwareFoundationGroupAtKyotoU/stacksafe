#ifndef INCLUDE_GUARD_24D0B7C2_7662_4CA7_9DAE_CFC9EA3B0EA0
#define INCLUDE_GUARD_24D0B7C2_7662_4CA7_9DAE_CFC9EA3B0EA0

#include "environment.hpp"
#include <memory>
#include <queue>
#include <unordered_map>
#include <vector>
#include <llvm/Pass.h>

namespace llvm {
  class BasicBlock;
  class Function;
  class TerminatorInst;
}

namespace stacksafe {
  class State {
    LocationFactory factory_;
    std::unordered_map<llvm::BasicBlock *, Environment> map_;
    std::queue<llvm::BasicBlock *> todo_;
  public:
    State(llvm::Function &F);
    void traverse();
  private:
    void propagate(const Environment &prev, llvm::TerminatorInst &I);
    Environment update(llvm::BasicBlock &B);
  };

  class Analyzer : public llvm::FunctionPass {
    std::unique_ptr<State> state_;
  public:
    static char ID;
  public:
    Analyzer();
    bool runOnFunction(llvm::Function &F) override;
  };

  class ApplyVisitor : public llvm::InstVisitor<ApplyVisitor, bool> {
    using RetTy = bool;
    Environment &env_;
  public:
    explicit ApplyVisitor(Environment &env);
    RetTy visitAllocaInst(llvm::AllocaInst &I);
    RetTy visitLoadInst(llvm::LoadInst &I);
    RetTy visitStoreInst(llvm::StoreInst &I);
    RetTy visitGetElementPtrInst(llvm::GetElementPtrInst &I);
    RetTy visitInstruction(llvm::Instruction &I);
  };
}

#endif//INCLUDE_GUARD_24D0B7C2_7662_4CA7_9DAE_CFC9EA3B0EA0
