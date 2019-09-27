#ifndef INCLUDE_GUARD_8CFD1EA5_B0CA_49A1_AE42_BA8D4F1F41DF
#define INCLUDE_GUARD_8CFD1EA5_B0CA_49A1_AE42_BA8D4F1F41DF

#include <llvm/IR/InstVisitor.h>

namespace stacksafe {
class Env;
class Memory;

class Verifier : public llvm::InstVisitor<Verifier, bool> {
  using RetTy = bool;
  using Super = llvm::InstVisitor<Verifier, RetTy>;
  static constexpr RetTy safe = true;
  static constexpr RetTy unsafe = false;
  const Memory& memory_;
  explicit Verifier(const Memory& e);

 public:
  static RetTy run(const llvm::BasicBlock* b, const Env& pred);
  RetTy visit(const llvm::BasicBlock& b);
  RetTy visitCallInst(llvm::CallInst& i);
  RetTy visitReturnInst(llvm::ReturnInst& i);
  RetTy visitInstruction(llvm::Instruction& i);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_8CFD1EA5_B0CA_49A1_AE42_BA8D4F1F41DF
