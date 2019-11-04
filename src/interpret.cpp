#include "interpret.hpp"
#include "log.hpp"
#include "utility.hpp"

namespace stacksafe {

Interpreter::Interpreter(const Cache &c, const Log &l, const Memory &m)
    : cache_{c}, log_{l}, mem_{m}, error_{false} {}
const Memory &Interpreter::memory() const {
  return mem_;
}
bool Interpreter::visit(const llvm::BasicBlock &b) {
  for (auto &&i : const_cast<llvm::BasicBlock &>(b)) {
    log_.print(i).print_nl();
    Super::visit(i);
  }
  return error_;
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
  binop(i, *lhs, *rhs);
}
auto Interpreter::visitExtractElementInst(llvm::ExtractElementInst &i)
    -> RetTy {
  auto src = i.getVectorOperand();
  assert(src && "invalid operand");
  cast(i, *src);
}
auto Interpreter::visitInsertElementInst(llvm::InsertElementInst &i) -> RetTy {
  auto lhs = i.getOperand(0);
  auto rhs = i.getOperand(1);
  assert(lhs && rhs && "invalid operand");
  binop(i, *lhs, *rhs);
}
auto Interpreter::visitShuffleVectorInst(llvm::ShuffleVectorInst &i) -> RetTy {
  auto lhs = i.getOperand(0);
  auto rhs = i.getOperand(1);
  assert(lhs && rhs && "invalid operand");
  binop(i, *lhs, *rhs);
}
auto Interpreter::visitExtractValue(llvm::ExtractValueInst &i) -> RetTy {
  auto src = i.getAggregateOperand();
  assert(src && "invalid operand");
  cast(i, *src);
}
auto Interpreter::visitInsertValue(llvm::InsertValueInst &i) -> RetTy {
  auto lhs = i.getAggregateOperand();
  auto rhs = i.getInsertedValueOperand();
  assert(lhs && rhs && "invalid operand");
  binop(i, *lhs, *rhs);
}
auto Interpreter::visitAllocaInst(llvm::AllocaInst &i) -> RetTy {
  alloc(i);
}
auto Interpreter::visitLoadInst(llvm::LoadInst &i) -> RetTy {
  auto src = i.getPointerOperand();
  assert(src && "invalid operand");
  load(i, *src);
}
auto Interpreter::visitStoreInst(llvm::StoreInst &i) -> RetTy {
  auto src = i.getValueOperand();
  auto dst = i.getPointerOperand();
  assert(src && dst && "invalid operand");
  store(*src, *dst);
}
auto Interpreter::visitAtomicCmpXchgInst(llvm::AtomicCmpXchgInst &i) -> RetTy {
  auto ptr = i.getPointerOperand();
  auto val = i.getNewValOperand();
  assert(ptr && val && "invalid operand");
  cmpxchg(i, *ptr, *val);
}
auto Interpreter::visitAtomicRMWInst(llvm::AtomicRMWInst &i) -> RetTy {
  auto ptr = i.getPointerOperand();
  auto val = i.getValOperand();
  assert(ptr && val && "invalid operand");
  cmpxchg(i, *ptr, *val);
}
auto Interpreter::visitGetElementPtrInst(llvm::GetElementPtrInst &i) -> RetTy {
  auto src = i.getPointerOperand();
  assert(src && "invalid operand");
  cast(i, *src);
}
auto Interpreter::visitCastInst(llvm::CastInst &i) -> RetTy {
  auto src = i.getOperand(0);
  assert(src && "invalid operand");
  cast(i, *src);
}
auto Interpreter::visitCmpInst(llvm::CmpInst &i) -> RetTy {
  constant(i);
}
auto Interpreter::visitPHINode(llvm::PHINode &i) -> RetTy {
  Params params;
  for (const auto &use : i.incoming_values()) {
    auto val = use.get();
    assert(val && "unknown phi node");
    params.insert(val);
  }
  phi(i, params);
}
auto Interpreter::visitSelectInst(llvm::SelectInst &i) -> RetTy {
  Params params;
  for (const auto val : {i.getTrueValue(), i.getFalseValue()}) {
    assert(val && "unknown select node");
    params.insert(val);
  }
  phi(i, params);
}
auto Interpreter::visitCallInst(llvm::CallInst &i) -> RetTy {
  Params params;
  for (const auto &use : i.args()) {
    auto arg = use.get();
    assert(arg && "unknown parameter");
    params.insert(arg);
  }
  call(i, params);
}
auto Interpreter::visitReturnInst(llvm::ReturnInst &i) -> RetTy {
  if (auto ret = i.getReturnValue()) {
    auto d = lookup(*ret);
    if (d.has_local()) {
      log_.error_return(d);
      error_ = true;
    }
  }
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
  if (key.is_global() && diff.has_local()) {
    log_.error_global(diff);
    error_ = true;
  } else if (!diff.empty()) {
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
void Interpreter::collect(const Symbol &symbol, Domain &done) const {
  if (done.merge(Domain{symbol})) {
    for (const auto &sym : lookup(symbol)) {
      collect(sym, done);
    }
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
  auto sym = Symbol::make(cache_.lookup(dst));
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
void Interpreter::phi(const llvm::Value &dst, const Params &params) {
  Domain dom;
  for (const auto &val : params) {
    dom.merge(lookup(*val));
  }
  insert(dst, dom);
}
void Interpreter::call(const llvm::Value &dst, const Params &params) {
  Domain dom;
  for (const auto &val : params) {
    for (const auto &sym : lookup(*val)) {
      collect(sym, dom);
    }
  }
  if (dom.has_local() && dom.includes(Domain::global())) {
    log_.error_call(dom);
    error_ = true;
  }
  for (const auto &sym : dom) {
    insert(sym, dom);
    insert(sym, Domain::global());
  }
  if (!check_voidfunc(dst)) {
    insert(dst, dom);
    insert(dst, Domain::global());
  }
}
void Interpreter::constant(const llvm::Value &dst) {
  insert(dst, Domain{});
}

}  // namespace stacksafe
