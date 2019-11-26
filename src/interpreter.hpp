#ifndef INCLUDE_GUARD_B3D1167F_2A1F_4D42_BE6F_DF2090D8F177
#define INCLUDE_GUARD_B3D1167F_2A1F_4D42_BE6F_DF2090D8F177

#include <llvm/IR/InstVisitor.h>

namespace stacksafe {
class Domain;
class Error;
class Log;
class Map;
class MutableEnv;
class Params;
class Value;

class Interpreter : public llvm::InstVisitor<Interpreter, void> {
  using RetTy = void;
  using Super = llvm::InstVisitor<Interpreter, RetTy>;
  const Log &log_;
  Error &error_;
  MutableEnv &env_;
  Map &map_;
  bool diff_;

 public:
  explicit Interpreter(const Log &l, Error &error, MutableEnv &env, Map &map);
  void reset();
  bool diff() const;
  void visit(const llvm::BasicBlock &b);
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
  RetTy visitReturnInst(llvm::ReturnInst &i);

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
  Domain lookup(const Value &key) const;
  Domain lookup(const llvm::Value &key) const;
  void insert(const Value &key, const Domain &val);
  void insert(const llvm::Instruction &key, const Domain &val);
  void collect(const Value &sym, Domain &done) const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_B3D1167F_2A1F_4D42_BE6F_DF2090D8F177
