#include "interpret.hpp"
#include "log.hpp"
#include "register.hpp"
#include "utility.hpp"

namespace stacksafe {

Interpreter::Interpreter(const Log &l, const Env &m) : log_{l}, env_{m} {}
const Env &Interpreter::env() const {
  return env_;
}
Safe Interpreter::visit(const llvm::BasicBlock &b) {
  log_.print_hr2().print(b).print_hr().print_nl();
  for (auto &&i : const_cast<llvm::BasicBlock &>(b)) {
    log_.print(i).print_nl();
    Super::visit(i);
  }
  return safe_;
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
  binop(i, lhs, rhs);
}
auto Interpreter::visitExtractElementInst(llvm::ExtractElementInst &i)
    -> RetTy {
  auto src = i.getVectorOperand();
  assert(src && "invalid operand");
  cast(i, src);
}
auto Interpreter::visitInsertElementInst(llvm::InsertElementInst &i) -> RetTy {
  auto lhs = i.getOperand(0);
  auto rhs = i.getOperand(1);
  assert(lhs && rhs && "invalid operand");
  binop(i, lhs, rhs);
}
auto Interpreter::visitShuffleVectorInst(llvm::ShuffleVectorInst &i) -> RetTy {
  auto lhs = i.getOperand(0);
  auto rhs = i.getOperand(1);
  assert(lhs && rhs && "invalid operand");
  binop(i, lhs, rhs);
}
auto Interpreter::visitExtractValue(llvm::ExtractValueInst &i) -> RetTy {
  auto src = i.getAggregateOperand();
  assert(src && "invalid operand");
  cast(i, src);
}
auto Interpreter::visitInsertValue(llvm::InsertValueInst &i) -> RetTy {
  auto lhs = i.getAggregateOperand();
  auto rhs = i.getInsertedValueOperand();
  assert(lhs && rhs && "invalid operand");
  binop(i, lhs, rhs);
}
auto Interpreter::visitAllocaInst(llvm::AllocaInst &i) -> RetTy {
  alloc(i);
}
auto Interpreter::visitLoadInst(llvm::LoadInst &i) -> RetTy {
  auto src = i.getPointerOperand();
  assert(src && "invalid operand");
  load(i, src);
}
auto Interpreter::visitStoreInst(llvm::StoreInst &i) -> RetTy {
  auto src = i.getValueOperand();
  auto dst = i.getPointerOperand();
  assert(src && dst && "invalid operand");
  store(src, dst);
}
auto Interpreter::visitAtomicCmpXchgInst(llvm::AtomicCmpXchgInst &i) -> RetTy {
  auto ptr = i.getPointerOperand();
  auto val = i.getNewValOperand();
  assert(ptr && val && "invalid operand");
  cmpxchg(i, ptr, val);
}
auto Interpreter::visitAtomicRMWInst(llvm::AtomicRMWInst &i) -> RetTy {
  auto ptr = i.getPointerOperand();
  auto val = i.getValOperand();
  assert(ptr && val && "invalid operand");
  cmpxchg(i, ptr, val);
}
auto Interpreter::visitGetElementPtrInst(llvm::GetElementPtrInst &i) -> RetTy {
  auto src = i.getPointerOperand();
  assert(src && "invalid operand");
  cast(i, src);
}
auto Interpreter::visitCastInst(llvm::CastInst &i) -> RetTy {
  auto src = i.getOperand(0);
  assert(src && "invalid operand");
  cast(i, src);
}
auto Interpreter::visitCmpInst(llvm::CmpInst &i) -> RetTy {
  constant(&i);
}
auto Interpreter::visitPHINode(llvm::PHINode &i) -> RetTy {
  Params params;
  for (const auto &use : i.incoming_values()) {
    auto arg = use.get();
    assert(arg && "unknown phi node");
    params.insert(arg);
  }
  phi(i, params);
}
auto Interpreter::visitSelectInst(llvm::SelectInst &i) -> RetTy {
  Params params;
  for (const auto arg : {i.getTrueValue(), i.getFalseValue()}) {
    assert(arg && "unknown select node");
    params.insert(arg);
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
    auto d = env_.lookup(*ret);
    if (has_local(d)) {
      log_.error_return(d);
      safe_.unsafe();
    }
  }
}
void Interpreter::insert(const Symbol &key, const Domain &val) {
  auto diff = val.minus(env_.lookup(key));
  env_.insert(key, diff);
  if (!key.is_local() && has_local(diff)) {
    log_.error_global(diff);
    safe_.unsafe();
  }
  log_.print(key, diff);
}
void Interpreter::insert(const Register &key, const Domain &val) {
  auto diff = val.minus(env_.lookup(key));
  env_.insert(key, diff);
  log_.print(key, diff);
}
void Interpreter::binop(const llvm::Instruction &dst, const Value &lhs,
                        const Value &rhs) {
  auto dom = Domain::get_empty();
  dom.merge(env_.lookup(lhs));
  dom.merge(env_.lookup(rhs));
  insert(dst, dom);
}
void Interpreter::alloc(const llvm::AllocaInst &dst) {
  auto reg = Symbol::get_local(dst);
  insert(reg, Domain::get_empty());
  insert(dst, Domain::get_singleton(reg));
}
void Interpreter::load(const llvm::Instruction &dst, const Value &src) {
  auto dom = Domain::get_empty();
  for (const auto &reg : env_.lookup(src)) {
    dom.merge(env_.lookup(reg));
  }
  insert(dst, dom);
}
void Interpreter::store(const Value &src, const Value &dst) {
  auto val = env_.lookup(src);
  for (const auto &ptr : env_.lookup(dst)) {
    insert(ptr, val);
  }
}
void Interpreter::cmpxchg(const llvm::Instruction &dst, const Value &ptr,
                          const Value &val) {
  load(dst, ptr);
  store(val, ptr);
}
void Interpreter::cast(const llvm::Instruction &dst, const Value &src) {
  insert(dst, env_.lookup(src));
}
void Interpreter::phi(const llvm::Instruction &dst, const Params &params) {
  auto dom = Domain::get_empty();
  for (const auto &arg : params) {
    dom.merge(env_.lookup(*arg));
  }
  insert(dst, dom);
}
void Interpreter::call(const llvm::CallInst &dst, const Params &params) {
  auto dom = Domain::get_empty();
  for (const auto &arg : params) {
    for (const auto &reg : env_.lookup(*arg)) {
      env_.collect(reg, dom);
    }
  }
  if (has_local(dom) && dom.includes(Domain::get_global())) {
    log_.error_call(dom);
    safe_.unsafe();
  }
  for (const auto &reg : dom) {
    insert(reg, dom);
    insert(reg, Domain::get_global());
  }
  if (!check_voidfunc(dst)) {
    insert(dst, dom);
    insert(dst, Domain::get_global());
  }
}
void Interpreter::constant(const Value &dst) {
  insert(dst, Domain::get_empty());
}
bool Interpreter::has_local(const Domain &dom) {
  for (const auto &sym : dom) {
    if (sym.is_local()) {
      return true;
    }
  }
  return false;
}

}  // namespace stacksafe
