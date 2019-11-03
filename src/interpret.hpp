#ifndef INCLUDE_GUARD_B3D1167F_2A1F_4D42_BE6F_DF2090D8F177
#define INCLUDE_GUARD_B3D1167F_2A1F_4D42_BE6F_DF2090D8F177

#include <llvm/IR/InstVisitor.h>
#include "memory.hpp"

namespace stacksafe {
class Cache;
class Env;
class Log;
class Params;

class Interpreter : public llvm::InstVisitor<Interpreter, void> {
  using RetTy = void;
  using Super = llvm::InstVisitor<Interpreter, RetTy>;
  const Cache &cache_;
  const Log &log_;
  Memory mem_;

 public:
  explicit Interpreter(const Cache &c, const Log &l);
  RetTy visit(const llvm::BasicBlock &b);
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
  Domain lookup(const Symbol &key) const;
  Domain lookup(const llvm::Value &key) const;
  void insert(const Symbol &key, const Domain &val);
  void insert(const llvm::Value &key, const Domain &val);
  void collect(const Symbol &symbol, Domain &done) const;
  void binop(const llvm::Value &dst, const llvm::Value &lhs,
             const llvm::Value &rhs);
  void alloc(const llvm::Value &dst);
  void load(const llvm::Value &dst, const llvm::Value &src);
  void store(const llvm::Value &src, const llvm::Value &dst);
  void cmpxchg(const llvm::Value &dst, const llvm::Value &ptr,
               const llvm::Value &val);
  void cast(const llvm::Value &dst, const llvm::Value &src);
  void phi(const llvm::Value &dst, const Params &params);
  void call(const llvm::Value &dst, const Params &params);
  void constant(const llvm::Value &dst);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_B3D1167F_2A1F_4D42_BE6F_DF2090D8F177
