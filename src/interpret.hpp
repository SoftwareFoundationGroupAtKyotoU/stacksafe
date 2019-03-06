#ifndef INCLUDE_GUARD_6D412E24_64BA_4C27_BEA5_DB1CAFCC48BD
#define INCLUDE_GUARD_6D412E24_64BA_4C27_BEA5_DB1CAFCC48BD

#include "environment.hpp"
#include <llvm/IR/InstVisitor.h>

namespace stacksafe {
  class Interpret : public llvm::InstVisitor<Interpret, bool> {
    using Base = llvm::InstVisitor<Interpret, bool>;
    Env &env_;
  public:
    explicit Interpret(Env &env);
    void visit(llvm::BasicBlock &B);
    bool visit(llvm::Instruction &I);
    bool visitInstruction(llvm::Instruction &I);
    bool visitAllocaInst(llvm::AllocaInst &I);
    bool visitStoreInst(llvm::StoreInst &I);
    bool visitLoadInst(llvm::LoadInst &I);
    bool visitGetElementPtrInst(llvm::GetElementPtrInst &I);
  private:
    static bool error(const char *name);
  };

  Env interpret(llvm::BasicBlock &b, Env env);
}

#endif//INCLUDE_GUARD_6D412E24_64BA_4C27_BEA5_DB1CAFCC48BD
