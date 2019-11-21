#include "interpreter.hpp"
#include "domain.hpp"
#include "env.hpp"
#include "error.hpp"
#include "log.hpp"
#include "utility.hpp"

namespace stacksafe {
namespace {
bool is_void_func(const llvm::CallInst &i) {
  return i.getFunctionType()->getReturnType()->isVoidTy();
}
bool has_local(const Domain &dom) {
  for (const auto &sym : dom) {
    if (sym.is_local()) {
      return true;
    }
  }
  return false;
}

class Params : private std::unordered_set<const llvm::Value *> {
  using Super = std::unordered_set<const llvm::Value *>;
  class ParamIterator : private Super::const_iterator {
    friend class Params;
    using Iter = Super::const_iterator;
    explicit ParamIterator(Iter it) : Iter{it} {}
    Iter &super() { return *this; }
    const Iter &super() const { return *this; }

   public:
    using Iter::iterator_category, Iter::value_type, Iter::difference_type,
        Iter::pointer, Iter::reference;
    const llvm::Value &operator*() const { return *Iter::operator*(); }
    ParamIterator &operator++() {
      ++super();
      return *this;
    }
  };

 public:
  ParamIterator begin() const { return ParamIterator{Super::begin()}; }
  ParamIterator end() const { return ParamIterator{Super::end()}; }
  void emplace(const llvm::Value &v) { Super::emplace(&v); }
};
}  // namespace

Interpreter::Interpreter(const Log &l, Error &error, const Map &heap,
                         const Map &stack)
    : log_{l}, error_{error}, heap_{heap}, stack_{stack} {}
const Map &Interpreter::heap_diff() const {
  return heap_diff_;
}
const Map &Interpreter::stack_diff() const {
  return stack_diff_;
}
void Interpreter::visit(const llvm::BasicBlock &b) {
  log_.print(b);
  for (auto &&i : const_cast<llvm::BasicBlock &>(b)) {
    log_.print(i);
    Super::visit(i);
    if (error_.is_error()) {
      log_.print(error_);
      return;
    }
  }
}
auto Interpreter::visitInstruction(llvm::Instruction &i) -> RetTy {
  if (!i.isTerminator()) {
    unsupported_instruction(i);
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
  ParamsOld params;
  for (const auto &use : i.incoming_values()) {
    auto arg = use.get();
    assert(arg && "unknown phi node");
    params.emplace(*arg);
  }
  phi(i, params);
}
auto Interpreter::visitSelectInst(llvm::SelectInst &i) -> RetTy {
  ParamsOld params;
  for (const auto arg : {i.getTrueValue(), i.getFalseValue()}) {
    assert(arg && "unknown select node");
    params.emplace(*arg);
  }
  phi(i, params);
}
auto Interpreter::visitCallInst(llvm::CallInst &i) -> RetTy {
  ParamsOld params;
  for (const auto &use : i.args()) {
    auto arg = use.get();
    assert(arg && "unknown parameter");
    params.emplace(*arg);
  }
  call(i, params);
}
auto Interpreter::visitReturnInst(llvm::ReturnInst &i) -> RetTy {
  if (auto ret = i.getReturnValue()) {
    if (has_local(stack_lookup(*ret))) {
      error_.error_return();
    }
  }
}
void Interpreter::binop(const llvm::Instruction &dst, const llvm::Value &lhs,
                        const llvm::Value &rhs) {
  Domain dom;
  dom.merge(stack_lookup(lhs));
  dom.merge(stack_lookup(rhs));
  stack_insert(dst, dom);
}
void Interpreter::alloc(const llvm::AllocaInst &dst) {
  Domain dom;
  const auto sym = Symbol::get_local(dst);
  heap_insert(sym, dom);
  dom.insert(sym);
  stack_insert(dst, dom);
}
void Interpreter::load(const llvm::Instruction &dst, const llvm::Value &src) {
  Domain dom;
  for (const auto &sym : stack_lookup(src)) {
    dom.merge(heap_lookup(sym));
  }
  stack_insert(dst, dom);
}
void Interpreter::store(const llvm::Value &src, const llvm::Value &dst) {
  const auto val = stack_lookup(src);
  for (const auto &ptr : stack_lookup(dst)) {
    heap_insert(ptr, val);
  }
}
void Interpreter::cmpxchg(const llvm::Instruction &dst, const llvm::Value &ptr,
                          const llvm::Value &val) {
  load(dst, ptr);
  store(val, ptr);
}
void Interpreter::cast(const llvm::Instruction &dst, const llvm::Value &src) {
  stack_insert(dst, stack_lookup(src));
}
void Interpreter::phi(const llvm::Instruction &dst, const ParamsOld &params) {
  Domain dom;
  for (const auto &arg : params) {
    dom.merge(stack_lookup(arg));
  }
  stack_insert(dst, dom);
}
void Interpreter::call(const llvm::CallInst &dst, const ParamsOld &params) {
  Domain dom;
  dom.insert(Symbol::get_global());
  for (const auto &arg : params) {
    for (const auto &sym : stack_lookup(arg)) {
      collect(sym, dom);
    }
  }
  if (has_local(dom)) {
    error_.error_call();
  }
  for (const auto &sym : dom) {
    if (!sym.is_global()) {
      heap_insert(sym, dom);
    }
  }
  if (!is_void_func(dst)) {
    stack_insert(dst, dom);
  }
}
void Interpreter::constant(const llvm::Instruction &dst) {
  stack_insert(dst, Domain{});
}
Domain Interpreter::heap_lookup(const Symbol &key) const {
  return heap_.lookup(key);
}
Domain Interpreter::stack_lookup(const Value &key) const {
  Domain dom;
  const auto v = key.get();
  if (!v) {
    return dom;
  } else if (auto c = llvm::dyn_cast<llvm::Constant>(v)) {
    if (is_global(*c)) {
      dom.insert(Symbol::get_global());
    }
    return dom;
  } else if (auto i = llvm::dyn_cast<llvm::Instruction>(v)) {
    assert(is_register(*i) && "invalid register lookup");
    return stack_.lookup(key);
  } else {
    assert(llvm::isa<llvm::Argument>(v) && "invalid value lookup");
    return stack_.lookup(key);
  }
}
void Interpreter::heap_insert(const Symbol &key, const Domain &val) {
  if (val.empty()) {
    return;
  }
  log_.print_heap(key.value(), heap_lookup(key), val);
  heap_.insert(key, val);
  heap_diff_.insert(key.value(), val);
  if (has_local(val)) {
    if (key.is_global()) {
      error_.error_global();
    }
    if (key.is_arg()) {
      error_.error_argument();
    }
  }
}
void Interpreter::stack_insert(const llvm::Instruction &key,
                               const Domain &val) {
  if (val.empty()) {
    return;
  }
  log_.print_stack(key, stack_lookup(key), val);
  stack_.insert(key, val);
  stack_diff_.insert(key, val);
}
void Interpreter::collect(const Symbol &sym, Domain &done) const {
  if (!done.element(sym)) {
    done.insert(sym);
    for (const auto &next : heap_lookup(sym)) {
      collect(next, done);
    }
  }
}

}  // namespace stacksafe
