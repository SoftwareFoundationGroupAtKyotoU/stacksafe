#include "interpret.hpp"
#include "env.hpp"
#include "log.hpp"
#include "utility.hpp"

namespace stacksafe {

Interpreter::Interpreter(const Cache &c, Env &e, const Log &l)
    : cache_{c}, env_{e}, log_{l} {}
auto Interpreter::visit(const llvm::BasicBlock &b) -> RetTy {
  for (auto &&i : const_cast<llvm::BasicBlock &>(b)) {
    Super::visit(i);
    log_.print(i);
    log_.print_nl();
  }
}
auto Interpreter::visitInstruction(llvm::Instruction &i) -> RetTy {
  if (!i.isTerminator()) {
    stacksafe_unreachable("unsupported instruction", i);
  }
}
auto Interpreter::visitBinaryOperator(llvm::BinaryOperator &i) -> RetTy {
  auto lhs = i.getOperand(0);
  auto rhs = i.getOperand(1);
  assert(lhs && rhs && "invalid operand");
  env_.binop(i, *lhs, *rhs);
}
auto Interpreter::visitExtractElementInst(llvm::ExtractElementInst &i)
    -> RetTy {
  auto src = i.getVectorOperand();
  assert(src && "invalid operand");
  env_.cast(i, *src);
}
auto Interpreter::visitInsertElementInst(llvm::InsertElementInst &i) -> RetTy {
  auto lhs = i.getOperand(0);
  auto rhs = i.getOperand(1);
  assert(lhs && rhs && "invalid operand");
  env_.binop(i, *lhs, *rhs);
}
auto Interpreter::visitShuffleVectorInst(llvm::ShuffleVectorInst &i) -> RetTy {
  auto lhs = i.getOperand(0);
  auto rhs = i.getOperand(1);
  assert(lhs && rhs && "invalid operand");
  env_.binop(i, *lhs, *rhs);
}
auto Interpreter::visitExtractValue(llvm::ExtractValueInst &i) -> RetTy {
  auto src = i.getAggregateOperand();
  assert(src && "invalid operand");
  env_.cast(i, *src);
}
auto Interpreter::visitInsertValue(llvm::InsertValueInst &i) -> RetTy {
  auto lhs = i.getAggregateOperand();
  auto rhs = i.getInsertedValueOperand();
  assert(lhs && rhs && "invalid operand");
  env_.binop(i, *lhs, *rhs);
}
auto Interpreter::visitAllocaInst(llvm::AllocaInst &i) -> RetTy {
  env_.alloc(i);
}
auto Interpreter::visitLoadInst(llvm::LoadInst &i) -> RetTy {
  auto src = i.getPointerOperand();
  assert(src && "invalid operand");
  env_.load(i, *src);
}
auto Interpreter::visitStoreInst(llvm::StoreInst &i) -> RetTy {
  auto src = i.getValueOperand();
  auto dst = i.getPointerOperand();
  assert(src && dst && "invalid operand");
  env_.store(*src, *dst);
}
auto Interpreter::visitAtomicCmpXchgInst(llvm::AtomicCmpXchgInst &i) -> RetTy {
  auto ptr = i.getPointerOperand();
  auto val = i.getNewValOperand();
  assert(ptr && val && "invalid operand");
  env_.cmpxchg(i, *ptr, *val);
}
auto Interpreter::visitAtomicRMWInst(llvm::AtomicRMWInst &i) -> RetTy {
  auto ptr = i.getPointerOperand();
  auto val = i.getValOperand();
  assert(ptr && val && "invalid operand");
  env_.cmpxchg(i, *ptr, *val);
}
auto Interpreter::visitGetElementPtrInst(llvm::GetElementPtrInst &i) -> RetTy {
  auto src = i.getPointerOperand();
  assert(src && "invalid operand");
  env_.cast(i, *src);
}
auto Interpreter::visitCastInst(llvm::CastInst &i) -> RetTy {
  auto src = i.getOperand(0);
  assert(src && "invalid operand");
  env_.cast(i, *src);
}
auto Interpreter::visitCmpInst(llvm::CmpInst &i) -> RetTy {
  env_.constant(i);
}
auto Interpreter::visitPHINode(llvm::PHINode &i) -> RetTy {
  Params params;
  for (const auto &use : i.incoming_values()) {
    auto val = use.get();
    assert(val && "unknown phi node");
    params.insert(*val);
  }
  env_.phi(i, params);
}
auto Interpreter::visitSelectInst(llvm::SelectInst &i) -> RetTy {
  Params params;
  for (const auto val : {i.getTrueValue(), i.getFalseValue()}) {
    assert(val && "unknown select node");
    params.insert(*val);
  }
  env_.phi(i, params);
}
auto Interpreter::visitCallInst(llvm::CallInst &i) -> RetTy {
  Params params;
  for (const auto &use : i.args()) {
    auto arg = use.get();
    assert(arg && "unknown parameter");
    params.insert(*arg);
  }
  env_.call(i, params);
}
Domain Interpreter::lookup(const Symbol &key) const {
  return mem_.lookup(key);
}
Domain Interpreter::lookup(const llvm::Value &key) const {
  if (check_register(key)) {
    return mem_.lookup(cache_.lookup(key));
  } else if (check_global(key)) {
    return Domain::global();
  } else if (check_constant(key)) {
    return Domain{};
  }
  stacksafe_unreachable("neither register nor constant", key);
}
void Interpreter::insert(const Symbol &key, const Domain &val) {
  auto diff = val.minus(mem_.lookup(key));
  mem_.insert(key, diff);
  if (!diff.empty()) {
    log_.print(key, diff);
  }
}
void Interpreter::insert(const llvm::Value &key, const Domain &val) {
  auto reg = cache_.lookup(key);
  auto diff = val.minus(mem_.lookup(reg));
  mem_.insert(reg, diff);
  if (!diff.empty()) {
    log_.print(reg, diff);
  }
}
void Interpreter::binop(const llvm::Value &dst, const llvm::Value &lhs,
                        const llvm::Value &rhs) {
  Domain dom;
  dom.merge(lookup(lhs));
  dom.merge(lookup(rhs));
  insert(dst, dom);
}
void Interpreter::alloc(const llvm::Value &dst) {
  auto sym = Symbol::make();
  insert(sym, Domain{});
  insert(dst, Domain{sym});
}
void Interpreter::load(const llvm::Value &dst, const llvm::Value &src) {
  Domain dom;
  for (const auto &sym : lookup(src)) {
    dom.merge(lookup(sym));
  }
  insert(dst, dom);
}
void Interpreter::store(const llvm::Value &src, const llvm::Value &dst) {
  auto source = lookup(src);
  for (const auto &target : lookup(dst)) {
    insert(target, source);
  }
}
void Interpreter::cmpxchg(const llvm::Value &dst, const llvm::Value &ptr,
                          const llvm::Value &val) {
  load(dst, ptr);
  store(val, ptr);
}
void Interpreter::cast(const llvm::Value &dst, const llvm::Value &src) {
  insert(dst, lookup(src));
}

}  // namespace stacksafe
