#ifndef INCLUDE_GUARD_8CFD1EA5_B0CA_49A1_AE42_BA8D4F1F41DF
#define INCLUDE_GUARD_8CFD1EA5_B0CA_49A1_AE42_BA8D4F1F41DF

#include <llvm/IR/InstVisitor.h>

namespace stacksafe {
class Env;

class Verifier : public llvm::InstVisitor<Verifier, bool> {
  using RetTy = bool;
  using Super = llvm::InstVisitor<Verifier, RetTy>;
  static constexpr RetTy safe = true;
  static constexpr RetTy unsafe = false;
  const Env& env_;

 public:
  explicit Verifier(const Env& e);
  RetTy visit(const llvm::BasicBlock& b);
  RetTy visitCallInst(llvm::CallInst& i);
  RetTy visitReturnInst(llvm::ReturnInst& i);
  RetTy visitInstruction(llvm::Instruction& i);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_8CFD1EA5_B0CA_49A1_AE42_BA8D4F1F41DF
