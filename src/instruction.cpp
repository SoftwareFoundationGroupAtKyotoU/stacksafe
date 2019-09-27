#include "instruction.hpp"
#include <llvm/IR/Value.h>
#include "env.hpp"
#include "memory.hpp"

namespace stacksafe {
namespace instr {

void alloc(Env& e, const llvm::Value& dst) {
  auto sym = Symbol::make(Type{dst.getType()}.pointee_type());
  e.insert_heap(sym, Domain{});
  e.insert_stack(dst, Domain{sym});
}
void load(Env& e, const llvm::Value& dst, const llvm::Value& src) {
  Domain dom;
  for (auto& sym : e.from_stack(src)) {
    dom.insert(e.from_heap(sym));
  }
  e.insert_stack(dst, dom);
}
void store(Env& e, const llvm::Value& src, const llvm::Value& dst) {
  auto source = e.from_stack(src);
  for (auto& target : e.from_stack(dst)) {
    e.insert_heap(target, source);
  }
}
void cmpxchg(Env& e, const llvm::Value& dst, const llvm::Value& ptr,
             const llvm::Value& val) {
  load(e, dst, ptr);
  store(e, val, ptr);
}
void cast(Env& e, const llvm::Value& dst, const llvm::Value& src) {
  e.insert_stack(dst, e.from_stack(src));
}
void phi(Env& e, const llvm::Value& dst, const ValueSet& params) {
  Domain dom;
  for (auto& val : params) {
    assert(val && "invalid param");
    dom.insert(e.from_stack(*val));
  }
  e.insert_stack(dst, dom);
}
void call(Env& e, const ValueSet& params) {
  auto dom = e.collect(params);
  for (auto& sym : dom) {
    e.insert_heap(sym, dom);
  }
}
void call(Env& e, const llvm::Value& dst, const ValueSet& params) {
  call(e, params);
  e.insert_stack(dst, e.collect(params));
}
void constant(Env& e, const llvm::Value& dst) { e.insert_stack(dst, Domain{}); }

}  // namespace instr
}  // namespace stacksafe
