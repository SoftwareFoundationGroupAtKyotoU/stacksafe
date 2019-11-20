#include "env.hpp"
#include "utility.hpp"

namespace stacksafe {

Env::Env(MapRef heap, MapRef stack) : heap_{heap}, stack_{stack} {}
Map Env::heap() const {
  Map ret;
  for (const auto &r : heap_) {
    ret.merge(r.get());
  }
  return ret;
}
Map Env::stack() const {
  Map ret;
  for (const auto &r : stack_) {
    ret.merge(r.get());
  }
  return ret;
}
void Env::merge(const Env &env) {
  heap_.insert(env.heap_.begin(), env.heap_.end());
  stack_.insert(env.stack_.begin(), env.stack_.end());
}
void Env::merge_heap(MapRef ref) {
  heap_.emplace(ref);
}
void Env::merge_stack(MapRef ref) {
  stack_.emplace(ref);
}
bool Env::includes(const Env &env) {
  auto compare = [](const auto &lhs, const auto &rhs) {
    Map lmap, rmap;
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
