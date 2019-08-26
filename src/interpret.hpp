#ifndef INCLUDE_GUARD_B3D1167F_2A1F_4D42_BE6F_DF2090D8F177
#define INCLUDE_GUARD_B3D1167F_2A1F_4D42_BE6F_DF2090D8F177

#include <llvm/IR/InstVisitor.h>

class Abstraction;

class Interpret : public llvm::InstVisitor<Interpret> {
  using Base = llvm::InstVisitor<Interpret>;
  Abstraction &abst_;

 public:
  explicit Interpret(Abstraction &a);
  void visit(llvm::Function &f);
  void visit(llvm::BasicBlock &b);
  void visit(llvm::Instruction &i);
  void visitAllocaInst(llvm::AllocaInst &i);
};

#endif  // INCLUDE_GUARD_B3D1167F_2A1F_4D42_BE6F_DF2090D8F177
