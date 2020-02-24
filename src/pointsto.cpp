#include "pointsto.hpp"
#include <llvm/Support/Debug.h>
#include "graph.hpp"
#include "params.hpp"
#include "utility.hpp"

namespace stacksafe {

PointsTo::PointsTo(Graph &g) : graph_{g}, updated_{false} {}
auto PointsTo::visitInstruction(llvm::Instruction &i) -> RetTy {
  if (!i.isTerminator()) {
    fatal_error("unsupported instruction: " + to_str(i));
  }
}
auto PointsTo::visitBinaryOperator(llvm::BinaryOperator &i) -> RetTy {
  auto lhs = i.getOperand(0);
  assert(lhs && "invalid operand");
  auto rhs = i.getOperand(1);
  assert(rhs && "invalid operand");
  binop(i, *lhs, *rhs);
}
auto PointsTo::visitExtractElementInst(llvm::ExtractElementInst &i) -> RetTy {
  auto src = i.getVectorOperand();
  assert(src && "invalid operand");
  cast(i, *src);
}
auto PointsTo::visitInsertElementInst(llvm::InsertElementInst &i) -> RetTy {
  auto lhs = i.getOperand(0);
  assert(lhs && "invalid operand");
  auto rhs = i.getOperand(1);
  assert(rhs && "invalid operand");
  binop(i, *lhs, *rhs);
}
auto PointsTo::visitShuffleVectorInst(llvm::ShuffleVectorInst &i) -> RetTy {
  auto lhs = i.getOperand(0);
  assert(lhs && "invalid operand");
  auto rhs = i.getOperand(1);
  assert(rhs && "invalid operand");
  binop(i, *lhs, *rhs);
}
auto PointsTo::visitExtractValue(llvm::ExtractValueInst &i) -> RetTy {
  auto src = i.getAggregateOperand();
  assert(src && "invalid operand");
  cast(i, *src);
}
auto PointsTo::visitInsertValue(llvm::InsertValueInst &i) -> RetTy {
  auto lhs = i.getAggregateOperand();
  assert(lhs && "invalid operand");
  auto rhs = i.getInsertedValueOperand();
  assert(rhs && "invalid operand");
  binop(i, *lhs, *rhs);
}
auto PointsTo::visitAllocaInst(llvm::AllocaInst &i) -> RetTy {
  alloc(i);
}
auto PointsTo::visitLoadInst(llvm::LoadInst &i) -> RetTy {
  auto src = i.getPointerOperand();
  assert(src && "invalid operand");
  load(i, *src);
}
auto PointsTo::visitStoreInst(llvm::StoreInst &i) -> RetTy {
  auto src = i.getValueOperand();
  assert(src && "invalid operand");
  auto dst = i.getPointerOperand();
  assert(dst && "invalid operand");
  store(*src, *dst);
}
auto PointsTo::visitAtomicCmpXchgInst(llvm::AtomicCmpXchgInst &i) -> RetTy {
  auto ptr = i.getPointerOperand();
  assert(ptr && "invalid operand");
  auto val = i.getNewValOperand();
  assert(val && "invalid operand");
  cmpxchg(i, *ptr, *val);
}
auto PointsTo::visitAtomicRMWInst(llvm::AtomicRMWInst &i) -> RetTy {
  auto ptr = i.getPointerOperand();
  assert(ptr && "invalid operand");
  auto val = i.getValOperand();
  assert(val && "invalid operand");
  cmpxchg(i, *ptr, *val);
}
auto PointsTo::visitGetElementPtrInst(llvm::GetElementPtrInst &i) -> RetTy {
  auto src = i.getPointerOperand();
  assert(src && "invalid operand");
  cast(i, *src);
}
auto PointsTo::visitCastInst(llvm::CastInst &i) -> RetTy {
  auto src = i.getOperand(0);
  assert(src && "invalid operand");
  cast(i, *src);
}
auto PointsTo::visitCmpInst(llvm::CmpInst &i) -> RetTy {
  constant(i);
}
auto PointsTo::visitPHINode(llvm::PHINode &i) -> RetTy {
  Params params;
  for (const auto &use : i.incoming_values()) {
    auto arg = use.get();
    assert(arg && "unknown phi node");
    params.push_back(*arg);
  }
  phi(i, params);
}
auto PointsTo::visitSelectInst(llvm::SelectInst &i) -> RetTy {
  Params params;
  for (const auto arg : {i.getTrueValue(), i.getFalseValue()}) {
    assert(arg && "unknown select node");
    params.push_back(*arg);
  }
  phi(i, params);
}
void PointsTo::binop(const llvm::Instruction &dst, const llvm::Value &lhs,
                     const llvm::Value &rhs) {
  NodeSet heads;
  heads.merge(lookup(lhs));
  heads.merge(lookup(rhs));
  append(dst, heads);
}
void PointsTo::alloc(const llvm::AllocaInst &dst) {
  NodeSet heads;
  heads.insert(Node{Symbol{dst}});
  append(dst, heads);
}
void PointsTo::load(const llvm::Instruction &dst, const llvm::Value &src) {
  NodeSet heads;
  for (const auto &sym : lookup(src)) {
    heads.merge(graph_.heads(sym));
  }
  append(dst, heads);
}
void PointsTo::store(const llvm::Value &src, const llvm::Value &dst) {
  const auto heads = lookup(src);
  for (const auto &tail : lookup(dst)) {
    for (const auto &head : heads) {
      update(graph_.append(tail, head));
    }
  }
}
void PointsTo::cmpxchg(const llvm::Instruction &dst, const llvm::Value &ptr,
                       const llvm::Value &val) {
  load(dst, ptr);
  store(val, ptr);
}
void PointsTo::cast(const llvm::Instruction &dst, const llvm::Value &src) {
  append(dst, lookup(src));
}
void PointsTo::phi(const llvm::Instruction &dst, const Params &params) {
  NodeSet heads;
  for (const auto &arg : params) {
    heads.merge(lookup(arg));
  }
  append(dst, heads);
}
void PointsTo::constant(const llvm::Instruction &) {}
NodeSet PointsTo::lookup(const Symbol &tail) const {
  return graph_.heads(Node{tail});
}
NodeSet PointsTo::lookup(const llvm::Value &tail) const {
  const auto v = &tail;
  if (auto c = llvm::dyn_cast<llvm::Constant>(v)) {
    NodeSet nodes;
    if (is_global(*c)) {
      nodes.insert(Node{Symbol::get_global()});
    }
    return nodes;
  } else if (auto i = llvm::dyn_cast<llvm::Instruction>(v)) {
    assert(is_register(*i) && "invalid register lookup");
    return graph_.heads(Node{Register{*i}});
  } else if (auto a = llvm::dyn_cast<llvm::Argument>(v)) {
    return graph_.heads(Node{Register{*a}});
  } else {
    llvm_unreachable("invalid value lookup");
  };
}
void PointsTo::append(const Symbol &tail, const NodeSet &heads) {
  for (const auto &h : heads) {
    update(graph_.append(Node{tail}, h));
  }
}
void PointsTo::append(const llvm::Instruction &tail, const NodeSet &heads) {
  for (const auto &h : heads) {
    update(graph_.append(Node{Register{tail}}, h));
  }
}
void PointsTo::update(bool updated) {
  if (updated) {
    updated_ = true;
  }
}

}  // namespace stacksafe
