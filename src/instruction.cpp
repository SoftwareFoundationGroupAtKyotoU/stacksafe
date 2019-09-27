#include "instruction.hpp"
#include "env.hpp"

namespace stacksafe {
namespace instr {

void binop(Memory& e, const Value& dst, const Value& lhs, const Value& rhs) {
  Domain dom;
  dom.insert(e.from_stack(lhs));
  dom.insert(e.from_stack(rhs));
  e.insert_stack(dst, dom);
}
void alloc(Memory& e, const Value& dst) {
  auto sym = Symbol::make(dst.type().pointee_type());
  e.insert_heap(sym, Domain{});
  e.insert_stack(dst, Domain{sym});
}
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

}  // namespace instr
}  // namespace stacksafe
