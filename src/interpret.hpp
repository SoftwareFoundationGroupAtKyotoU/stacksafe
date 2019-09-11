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
  static Env run(const llvm::BasicBlock *b, const Env &pred);
  void visit(llvm::BasicBlock &b);
  RetTy visit(llvm::Instruction &i);
  RetTy visitInstruction(llvm::Instruction &i);
  RetTy visitBinaryOperator(llvm::BinaryOperator &i);
  RetTy visitAllocaInst(llvm::AllocaInst &i);
  RetTy visitLoadInst(llvm::LoadInst &i);
  RetTy visitStoreInst(llvm::StoreInst &i);
  RetTy visitAtomicCmpXchgInst(llvm::AtomicCmpXchgInst &i);
  RetTy visitAtomicRMWInst(llvm::AtomicRMWInst &i);
  RetTy visitCastInst(llvm::CastInst &i);
  RetTy visitCmpInst(llvm::CmpInst &i);
  RetTy visitPHINode(llvm::PHINode &i);
  RetTy visitCallInst(llvm::CallInst &i);
  RetTy visitGetElementPtrInst(llvm::GetElementPtrInst &i);
  RetTy visitSelectInst(llvm::SelectInst &i);
  RetTy visitExtractValue(llvm::ExtractValueInst &i);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_B3D1167F_2A1F_4D42_BE6F_DF2090D8F177
