#ifndef INCLUDE_GUARD_E3BE8F38_E950_4162_9837_04D50D8E97C6
#define INCLUDE_GUARD_E3BE8F38_E950_4162_9837_04D50D8E97C6

#include <llvm/IR/InstVisitor.h>

namespace stacksafe {
class Graph;
class NodeSet;
class Params;
class Symbol;

class PointsTo : public llvm::InstVisitor<PointsTo, void> {
  using RetTy = void;
  using Super = llvm::InstVisitor<PointsTo, RetTy>;
  Graph &graph_;
  bool updated_;

 public:
  explicit PointsTo(Graph &g);

 private:
  void binop(const llvm::Instruction &dst, const llvm::Value &lhs,
             const llvm::Value &rhs);
  void alloc(const llvm::AllocaInst &dst);
  void load(const llvm::Instruction &dst, const llvm::Value &src);
  void store(const llvm::Value &src, const llvm::Value &dst);
  void cmpxchg(const llvm::Instruction &dst, const llvm::Value &ptr,
               const llvm::Value &val);
  void cast(const llvm::Instruction &dst, const llvm::Value &src);
  void phi(const llvm::Instruction &dst, const Params &params);
  void constant(const llvm::Instruction &dst);

 private:
  NodeSet lookup(const Symbol &tail) const;
  NodeSet lookup(const llvm::Value &tail) const;
  void append(const Symbol &tail, const NodeSet &heads);
  void append(const llvm::Instruction &tail, const NodeSet &heads);
  void update(bool updated);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_E3BE8F38_E950_4162_9837_04D50D8E97C6
