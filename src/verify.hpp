#ifndef INCLUDE_GUARD_8CFD1EA5_B0CA_49A1_AE42_BA8D4F1F41DF
#define INCLUDE_GUARD_8CFD1EA5_B0CA_49A1_AE42_BA8D4F1F41DF

#include <llvm/IR/InstVisitor.h>

namespace stacksafe {
class Env;

class Verifier : public llvm::InstVisitor<Verifier, bool> {
  using RetTy = bool;
  using Super = llvm::InstVisitor<Verifier, RetTy>;
  const Env& env_;
  explicit Verifier(const Env& e);

 public:
  static RetTy run(const llvm::BasicBlock* b, const Env& pred);
  RetTy visitInstruction(llvm::Instruction& i);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_8CFD1EA5_B0CA_49A1_AE42_BA8D4F1F41DF
