#include "instruction.hpp"
#include <cassert>
#include "env.hpp"
#include "memory.hpp"

namespace stacksafe {
namespace instr {

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
