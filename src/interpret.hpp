#ifndef INCLUDE_GUARD_B3D1167F_2A1F_4D42_BE6F_DF2090D8F177
#define INCLUDE_GUARD_B3D1167F_2A1F_4D42_BE6F_DF2090D8F177

#include <llvm/IR/InstVisitor.h>
#include "env.hpp"

namespace stacksafe {

class Interpret : public llvm::InstVisitor<Interpret> {
  using Base = llvm::InstVisitor<Interpret>;
  Env abst_;

 public:
  Interpret() = default;
  void visit(llvm::Function &f);
  void visit(llvm::BasicBlock &b);
  void visit(llvm::Instruction &i);
  void visitAllocaInst(llvm::AllocaInst &i);
  void visitStoreInst(llvm::StoreInst &i);
  void visitLoadInst(llvm::LoadInst &i);
  void visitBinaryOperator(llvm::BinaryOperator &i);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_B3D1167F_2A1F_4D42_BE6F_DF2090D8F177
