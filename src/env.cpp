#include "env.hpp"
#include <llvm/IR/Function.h>
#include "utility.hpp"

namespace stacksafe {

FlatEnv::FlatEnv(const llvm::Function &f) {
  const auto g = Symbol::get_global();
  heap_.insert(g, g);
  for (const auto &a : f.args()) {
    const auto arg = Symbol::get_arg(a);
    heap_.insert(arg, arg);
    stack_.insert(a, arg);
  }
}
FlatEnv::FlatEnv(const FlatMap &heap, const FlatMap &stack)
    : heap_{heap.to_multi()}, stack_{stack.to_multi()} {}
const MultiMap &FlatEnv::heap() const {
  return heap_;
}
const MultiMap &FlatEnv::stack() const {
  return stack_;
}

Env::Env(MapRef heap, MapRef stack) : heap_{heap}, stack_{stack} {}
MultiMap Env::heap() const {
  MultiMap ret;
  for (const auto &r : heap_) {
    ret.merge(r.get());
  }
  return ret;
}
MultiMap Env::stack() const {
  MultiMap ret;
  for (const auto &r : stack_) {
    ret.merge(r.get());
  }
  return ret;
}
void Env::merge(const Env &env) {
  heap_.insert(env.heap_.begin(), env.heap_.end());
  stack_.insert(env.stack_.begin(), env.stack_.end());
}
bool Env::includes(const Env &env) {
  auto compare = [](const auto &lhs, const auto &rhs) {
    MultiMap lmap, rmap;
    for (const auto &ref : lhs) {
      if (rhs.count(ref) == 0) {
        lmap.merge(ref.get());
      }
    }
    for (const auto &ref : rhs) {
      if (lhs.count(ref) == 0) {
        rmap.merge(ref.get());
      }
    }
    return lmap.includes(rmap);
  };
  return compare(heap_, env.heap_) && compare(stack_, env.stack_);
}

}  // namespace stacksafe
