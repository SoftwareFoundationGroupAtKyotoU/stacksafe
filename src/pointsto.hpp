#ifndef INCLUDE_GUARD_E3BE8F38_E950_4162_9837_04D50D8E97C6
#define INCLUDE_GUARD_E3BE8F38_E950_4162_9837_04D50D8E97C6

#include <llvm/IR/InstVisitor.h>

namespace stacksafe {

class PointsTo : public llvm::InstVisitor<PointsTo, void> {
  using RetTy = void;
  using Super = llvm::InstVisitor<PointsTo, RetTy>;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_E3BE8F38_E950_4162_9837_04D50D8E97C6
