#ifndef INCLUDE_GUARD_8CFD1EA5_B0CA_49A1_AE42_BA8D4F1F41DF
#define INCLUDE_GUARD_8CFD1EA5_B0CA_49A1_AE42_BA8D4F1F41DF

#include <llvm/IR/InstVisitor.h>

namespace stacksafe {
class Env;

class Verifier : public llvm::InstVisitor<Verifier, void> {
  const Env& env_;
  explicit Verifier(const Env& e);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_8CFD1EA5_B0CA_49A1_AE42_BA8D4F1F41DF
