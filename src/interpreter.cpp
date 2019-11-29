#include "interpreter.hpp"
#include <llvm/Support/Debug.h>
#include "depend.hpp"
#include "domain.hpp"
#include "log.hpp"
#include "map.hpp"
#include "params.hpp"
#include "utility.hpp"

#define STACKSAFE_DEBUG_LOG(...) \
  DEBUG_WITH_TYPE("log", (log_ && log_.print(__VA_ARGS__)))

namespace stacksafe {

Interpreter::Interpreter(const Log &l, Depend &d, Map &m)
    : log_{l}, depend_{d}, map_{m} {}
bool Interpreter::visit(const llvm::BasicBlock &b) {
  diff_ = false;
  STACKSAFE_DEBUG_LOG(b);
  for (auto &&i : const_cast<llvm::BasicBlock &>(b)) {
    STACKSAFE_DEBUG_LOG(i);
    Super::visit(i);
    if (depend_.is_error()) {
      STACKSAFE_DEBUG_LOG(depend_);
      break;
    }
  }
  return diff_;
}
auto Interpreter::visitInstruction(llvm::Instruction &i) -> RetTy {
  if (!i.isTerminator()) {
    fatal_error("unsupported instruction: " + to_str(i));
  }
}
auto Interpreter::visitBinaryOperator(llvm::BinaryOperator &i) -> RetTy {
  auto lhs = i.getOperand(0);
  assert(lhs && "invalid operand");
  auto rhs = i.getOperand(1);
  assert(rhs && "invalid operand");
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
  assert(lhs && "invalid operand");
  auto rhs = i.getOperand(1);
  assert(rhs && "invalid operand");
  binop(i, *lhs, *rhs);
}
auto Interpreter::visitShuffleVectorInst(llvm::ShuffleVectorInst &i) -> RetTy {
  auto lhs = i.getOperand(0);
  assert(lhs && "invalid operand");
  auto rhs = i.getOperand(1);
  assert(rhs && "invalid operand");
  binop(i, *lhs, *rhs);
}
auto Interpreter::visitExtractValue(llvm::ExtractValueInst &i) -> RetTy {
  auto src = i.getAggregateOperand();
  assert(src && "invalid operand");
  cast(i, *src);
}
auto Interpreter::visitInsertValue(llvm::InsertValueInst &i) -> RetTy {
  auto lhs = i.getAggregateOperand();
  assert(lhs && "invalid operand");
  auto rhs = i.getInsertedValueOperand();
  assert(rhs && "invalid operand");
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
  assert(src && "invalid operand");
  auto dst = i.getPointerOperand();
  assert(dst && "invalid operand");
  store(*src, *dst);
}
auto Interpreter::visitAtomicCmpXchgInst(llvm::AtomicCmpXchgInst &i) -> RetTy {
  auto ptr = i.getPointerOperand();
  assert(ptr && "invalid operand");
  auto val = i.getNewValOperand();
  assert(val && "invalid operand");
  cmpxchg(i, *ptr, *val);
}
auto Interpreter::visitAtomicRMWInst(llvm::AtomicRMWInst &i) -> RetTy {
  auto ptr = i.getPointerOperand();
  assert(ptr && "invalid operand");
  auto val = i.getValOperand();
  assert(val && "invalid operand");
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
    auto arg = use.get();
    assert(arg && "unknown phi node");
    params.push_back(*arg);
  }
  phi(i, params);
}
auto Interpreter::visitSelectInst(llvm::SelectInst &i) -> RetTy {
  Params params;
  for (const auto arg : {i.getTrueValue(), i.getFalseValue()}) {
    assert(arg && "unknown select node");
    params.push_back(*arg);
  }
  phi(i, params);
}
auto Interpreter::visitCallInst(llvm::CallInst &i) -> RetTy {
  Params params;
  for (const auto &use : i.args()) {
    auto arg = use.get();
    assert(arg && "unknown parameter");
    params.push_back(*arg);
  }
  call(i, params);
}
auto Interpreter::visitReturnInst(llvm::ReturnInst &i) -> RetTy {
  if (auto ret = i.getReturnValue()) {
    for (const auto &sym : lookup(*ret)) {
      depend_.set_return(sym);
    }
  }
}
void Interpreter::binop(const llvm::Instruction &dst, const llvm::Value &lhs,
                        const llvm::Value &rhs) {
  Domain dom;
  dom.merge(lookup(lhs));
  dom.merge(lookup(rhs));
  insert(dst, dom);
}
void Interpreter::alloc(const llvm::AllocaInst &dst) {
  const Symbol sym{dst};
  if (lookup(dst).empty()) {
    insert(sym, Domain{});
    insert(dst, Domain{sym});
  } else {
    fatal_error("dynamic alloca instruction is not supported");
  }
}
void Interpreter::load(const llvm::Instruction &dst, const llvm::Value &src) {
  Domain dom;
  for (const auto &sym : lookup(src)) {
    dom.merge(lookup(sym));
  }
  insert(dst, dom);
}
void Interpreter::store(const llvm::Value &src, const llvm::Value &dst) {
  const auto val = lookup(src);
  for (const auto &ptr : lookup(dst)) {
    insert(ptr, val);
  }
}
void Interpreter::cmpxchg(const llvm::Instruction &dst, const llvm::Value &ptr,
                          const llvm::Value &val) {
  load(dst, ptr);
  store(val, ptr);
}
void Interpreter::cast(const llvm::Instruction &dst, const llvm::Value &src) {
  insert(dst, lookup(src));
}
void Interpreter::phi(const llvm::Instruction &dst, const Params &params) {
  Domain dom;
  for (const auto &arg : params) {
    dom.merge(lookup(arg));
  }
  insert(dst, dom);
}
void Interpreter::call(const llvm::CallInst &dst, const Params &params) {
  Domain dom{Symbol::get_global()};
  for (const auto &arg : params) {
    for (const auto &val : lookup(arg)) {
      collect(val, dom);
    }
  }
  for (const auto &val : dom) {
    insert(val, dom);
  }
  if (is_return(dst)) {
    insert(dst, dom);
  }
}
void Interpreter::constant(const llvm::Instruction &dst) {
  Domain dom;
  insert(dst, dom);
}
Domain Interpreter::lookup(const Symbol &key) const {
  return map_.lookup(key);
}
Domain Interpreter::lookup(const llvm::Value &key) const {
  if (auto c = llvm::dyn_cast<llvm::Constant>(&key)) {
    return is_global(*c) ? Domain{Symbol::get_global()} : Domain{};
  } else if (auto i = llvm::dyn_cast<llvm::Instruction>(&key)) {
    assert(is_register(*i) && "invalid register lookup");
    return map_.lookup(Register{*i});
  } else if (auto a = llvm::dyn_cast<llvm::Argument>(&key)) {
    return map_.lookup(Register{*a});
  } else {
    llvm_unreachable("invalid value lookup");
  }
}
void Interpreter::insert(const Symbol &key, const Domain &dom) {
  if (!dom.empty()) {
    STACKSAFE_DEBUG_LOG(key, lookup(key), dom);
    update(map_.insert(key, dom));
    for (const auto &sym : dom) {
      depend_.set(key, sym);
    }
  }
}
void Interpreter::insert(const llvm::Instruction &key, const Domain &dom) {
  if (!dom.empty()) {
    const Register reg{key};
    STACKSAFE_DEBUG_LOG(reg, lookup(key), dom);
    update(map_.insert(reg, dom));
  }
}
void Interpreter::collect(const Symbol &sym, Domain &done) const {
  if (!done.element(sym)) {
    done.insert(sym);
    for (const auto &next : lookup(sym)) {
      collect(next, done);
    }
  }
}
void Interpreter::update(bool updated) {
  if (updated) {
    diff_ = true;
  }
}

}  // namespace stacksafe
