#include "pointsto.hpp"
#include "block.hpp"
#include "component.hpp"
#include "params.hpp"
#include "utility.hpp"

namespace stacksafe {

PointsTo::PointsTo(Component &c) : comp_{c} {}
void PointsTo::analyze(Component &c) {
  const auto &b = c.blocks();
  PointsTo p{c};
  auto prev = c.size();
  do {
    for (auto &&bb : b) {
      p.visit(const_cast<llvm::BasicBlock *>(bb));
    }
  } while (b.is_loop() && std::exchange(prev, c.size()) != c.size());
}
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
auto PointsTo::visitCallInst(llvm::CallInst &i) -> RetTy {
  Params params;
  for (const auto &use : i.args()) {
    auto arg = use.get();
    assert(arg && "unknown parameter");
    params.push_back(*arg);
  }
  call(i, params);
}
void PointsTo::binop(const llvm::Instruction &dst, const llvm::Value &lhs,
                     const llvm::Value &rhs) {
  NodeSet heads;
  comp_.followings(lhs, heads);
  comp_.followings(rhs, heads);
  comp_.connect(dst, heads);
}
void PointsTo::alloc(const llvm::AllocaInst &dst) {
  comp_.connect(dst, NodeSet{Node::get_local(dst)});
}
void PointsTo::load(const llvm::Instruction &dst, const llvm::Value &src) {
  NodeSet tails, heads;
  comp_.followings(src, tails);
  comp_.followings(tails, heads);
  comp_.connect(dst, heads);
}
void PointsTo::store(const llvm::Value &src, const llvm::Value &dst) {
  NodeSet tails, heads;
  comp_.followings(dst, tails);
  comp_.followings(src, heads);
  comp_.connect(tails, heads);
}
void PointsTo::cmpxchg(const llvm::Instruction &dst, const llvm::Value &ptr,
                       const llvm::Value &val) {
  load(dst, ptr);
  store(val, ptr);
}
void PointsTo::cast(const llvm::Instruction &dst, const llvm::Value &src) {
  NodeSet heads;
  comp_.followings(src, heads);
  comp_.connect(dst, heads);
}
void PointsTo::phi(const llvm::Instruction &dst, const Params &params) {
  NodeSet heads;
  for (const auto &arg : params) {
    comp_.followings(arg, heads);
  }
  comp_.connect(dst, heads);
}
void PointsTo::call(const llvm::CallInst &dst, const Params &params) {
  assert(dst.arg_size() == params.size());
  const auto effect = effects_.get(dst);
  NodeMap nodemap{params, comp_};
  for (const auto &[from, heads] : nodemap) {
    for (const auto &[to, tails] : nodemap) {
      if (effect.depends(from, to)) {
        comp_.connect(tails, heads);
      }
    }
    if (is_return(dst) && effect.depends(from, Index::RETURN)) {
      comp_.connect(dst, heads);
    }
  }
}
void PointsTo::constant(const llvm::Instruction &) {}

}  // namespace stacksafe
