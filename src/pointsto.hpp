#ifndef INCLUDE_GUARD_E3BE8F38_E950_4162_9837_04D50D8E97C6
#define INCLUDE_GUARD_E3BE8F38_E950_4162_9837_04D50D8E97C6

#include <llvm/IR/InstVisitor.h>

namespace stacksafe {
class Graph;
class NodeSet;
class Symbol;

class PointsTo : public llvm::InstVisitor<PointsTo, void> {
  using RetTy = void;
  using Super = llvm::InstVisitor<PointsTo, RetTy>;
  Graph& graph_;

 public:
  explicit PointsTo(Graph& g);

 private:
  NodeSet lookup(const Symbol& tail) const;
  NodeSet lookup(const llvm::Value& tail) const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_E3BE8F38_E950_4162_9837_04D50D8E97C6
