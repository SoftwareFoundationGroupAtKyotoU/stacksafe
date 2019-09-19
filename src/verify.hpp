#ifndef INCLUDE_GUARD_8CFD1EA5_B0CA_49A1_AE42_BA8D4F1F41DF
#define INCLUDE_GUARD_8CFD1EA5_B0CA_49A1_AE42_BA8D4F1F41DF

#include <llvm/IR/InstVisitor.h>

namespace stacksafe {

class Verifier : public llvm::InstVisitor<Verifier, void> {};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_8CFD1EA5_B0CA_49A1_AE42_BA8D4F1F41DF
