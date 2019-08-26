#ifndef INCLUDE_GUARD_B3D1167F_2A1F_4D42_BE6F_DF2090D8F177
#define INCLUDE_GUARD_B3D1167F_2A1F_4D42_BE6F_DF2090D8F177

#include <llvm/IR/InstVisitor.h>

class Interpret : public llvm::InstVisitor<Interpret> {
  using Base = llvm::InstVisitor<Interpret>;

 public:
  void visit(llvm::Function &f);
  void visit(llvm::BasicBlock &b);
  void visit(llvm::Instruction &i);
};

#endif  // INCLUDE_GUARD_B3D1167F_2A1F_4D42_BE6F_DF2090D8F177
