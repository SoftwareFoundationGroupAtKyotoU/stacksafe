#include "instruction.hpp"
#include <llvm/IR/Value.h>
#include "env.hpp"
#include "memory.hpp"

namespace stacksafe {
namespace instr {

void load(Memory& e, const Value& dst, const Value& src) {
  Domain dom;
  for (auto& sym : e.from_stack(src)) {
    dom.insert(e.from_heap(sym));
  }
  e.insert_stack(dst, dom);
}
void store(Memory& e, const Value& src, const Value& dst) {
  auto source = e.from_stack(src);
  for (auto& target : e.from_stack(dst)) {
    e.insert_heap(target, source);
  }
}
void cmpxchg(Memory& e, const Value& dst, const Value& ptr, const Value& val) {
  load(e, dst, ptr);
  store(e, val, ptr);
}
void cast(Memory& e, const Value& dst, const Value& src) {
  e.insert_stack(dst, e.from_stack(src));
}
void phi(Memory& e, const Value& dst, const Params& params) {
  Domain dom;
  for (auto& val : params) {
    dom.insert(e.from_stack(val));
  }
  e.insert_stack(dst, dom);
}
void call(Memory& e, const Params& params) { e.call(e.collect(params)); }
void call(Memory& e, const Value& dst, const Params& params) {
  auto dom = e.collect(params);
  e.call(dom);
  e.insert_stack(dst, dom);
}
void constant(Memory& e, const Value& dst) { e.insert_stack(dst, Domain{}); }

void binop(Env& e, const llvm::Value& dst, const llvm::Value& lhs,
           const llvm::Value& rhs) {
  Domain dom;
  dom.insert(e.from_stack(lhs));
  dom.insert(e.from_stack(rhs));
  e.insert_stack(dst, dom);
}
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
void phi(Env& e, const llvm::Value& dst, const Params& params) {
  Domain dom;
  for (auto& val : params) {
    dom.insert(e.from_stack(*val.get()));
  }
  e.insert_stack(dst, dom);
}
void call(Env& e, const Params& params) {
  auto dom = e.collect(params);
  for (auto& sym : dom) {
    e.insert_heap(sym, dom);
  }
}
void call(Env& e, const llvm::Value& dst, const Params& params) {
  call(e, params);
  e.insert_stack(dst, e.collect(params));
}
void constant(Env& e, const llvm::Value& dst) { e.insert_stack(dst, Domain{}); }

}  // namespace instr
}  // namespace stacksafe
