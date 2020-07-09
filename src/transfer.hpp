#ifndef INCLUDE_GUARD_ECA0C9FA_827B_4E7E_8CE9_9A0F858FBF7F
#define INCLUDE_GUARD_ECA0C9FA_827B_4E7E_8CE9_9A0F858FBF7F

#include <llvm/IR/InstVisitor.h>

namespace dataflow {
class Transfer : public llvm::InstVisitor<Transfer, void> {
  using RetTy = void;
  using Super = llvm::InstVisitor<Transfer, RetTy>;

 public:
  RetTy visitInstruction(llvm::Instruction &i);
};
}  // namespace dataflow

#endif  // INCLUDE_GUARD_ECA0C9FA_827B_4E7E_8CE9_9A0F858FBF7F
