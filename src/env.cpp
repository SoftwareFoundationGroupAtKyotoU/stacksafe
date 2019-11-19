#include "env.hpp"
#include <llvm/IR/Function.h>
#include "map.hpp"
#include "utility.hpp"

namespace stacksafe {

FlatEnv::FlatEnv(const llvm::Function &f) {
  const auto g = Symbol::get_global();
  heap_.insert(g.value(), g);
  for (const auto &a : f.args()) {
    const auto arg = Symbol::get_arg(a);
    heap_.insert(arg.value(), arg);
    stack_.insert(a, arg);
  }
}
FlatEnv::FlatEnv(const FlatMap &heap, const FlatMap &stack)
    : heap_{heap}, stack_{stack} {}
const FlatMap &FlatEnv::heap() const {
  return heap_;
}
const FlatMap &FlatEnv::stack() const {
  return stack_;
}

Env::Env(FlatMapRef heap, FlatMapRef stack) : heap_{heap}, stack_{stack} {}
Heap Env::heap() const {
  Map ret;
  for (const auto &r : heap_) {
    ret.merge(FlatMap::to_map(r.get()));
  }
  return Heap{ret};
}
Stack Env::stack() const {
  Map ret;
  for (const auto &r : stack_) {
    ret.merge(FlatMap::to_map(r.get()));
  }
  return Stack{ret};
}
void Env::merge(const Env &env) {
  heap_.insert(env.heap_.begin(), env.heap_.end());
  stack_.insert(env.stack_.begin(), env.stack_.end());
}
bool Env::includes(const Env &env) {
  auto compare = [](const auto &lhs, const auto &rhs) {
    FlatMap lmap, rmap;
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
