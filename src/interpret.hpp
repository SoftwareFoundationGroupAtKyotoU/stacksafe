#ifndef INCLUDE_GUARD_B3D1167F_2A1F_4D42_BE6F_DF2090D8F177
#define INCLUDE_GUARD_B3D1167F_2A1F_4D42_BE6F_DF2090D8F177

#include <llvm/IR/InstVisitor.h>

namespace stacksafe {
class Env;

class Interpret : public llvm::InstVisitor<Interpret, void> {
  using RetTy = void;
  using Base = llvm::InstVisitor<Interpret, RetTy>;
  Env &env_;

 public:
  explicit Interpret(Env &e);
  void visit(llvm::BasicBlock &b);
  RetTy visit(llvm::Instruction &i);
  RetTy visitInstruction(llvm::Instruction &i);
  RetTy visitAllocaInst(llvm::AllocaInst &i);
  RetTy visitStoreInst(llvm::StoreInst &i);
  RetTy visitLoadInst(llvm::LoadInst &i);
  RetTy visitBinaryOperator(llvm::BinaryOperator &i);
  RetTy visitCmpInst(llvm::CmpInst &i);
  RetTy visitCallInst(llvm::CallInst &i);
  RetTy visitCastInst(llvm::CastInst &i);
  RetTy visitGetElementPtrInst(llvm::GetElementPtrInst &i);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_B3D1167F_2A1F_4D42_BE6F_DF2090D8F177
