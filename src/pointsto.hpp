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
  void analyze(const llvm::Instruction &i);
  void reset();
  bool updated() const;
  RetTy visitInstruction(llvm::Instruction &i);
  RetTy visitBinaryOperator(llvm::BinaryOperator &i);
  RetTy visitExtractElementInst(llvm::ExtractElementInst &i);
  RetTy visitInsertElementInst(llvm::InsertElementInst &i);
  RetTy visitShuffleVectorInst(llvm::ShuffleVectorInst &i);
  RetTy visitExtractValue(llvm::ExtractValueInst &i);
  RetTy visitInsertValue(llvm::InsertValueInst &i);
  RetTy visitAllocaInst(llvm::AllocaInst &i);
  RetTy visitLoadInst(llvm::LoadInst &i);
  RetTy visitStoreInst(llvm::StoreInst &i);
  RetTy visitAtomicCmpXchgInst(llvm::AtomicCmpXchgInst &i);
  RetTy visitAtomicRMWInst(llvm::AtomicRMWInst &i);
  RetTy visitGetElementPtrInst(llvm::GetElementPtrInst &i);
  RetTy visitCastInst(llvm::CastInst &i);
  RetTy visitCmpInst(llvm::CmpInst &i);
  RetTy visitPHINode(llvm::PHINode &i);
  RetTy visitSelectInst(llvm::SelectInst &i);
  RetTy visitCallInst(llvm::CallInst &i);

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
  void call(const llvm::CallInst &dst, const Params &params);
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
