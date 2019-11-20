#include "env.hpp"
#include "utility.hpp"

namespace stacksafe {
namespace {
using RefSet = std::unordered_set<MapRef>;
Map diff_merge(const RefSet &lhs, const RefSet &rhs) {
  Map map;
  for (const auto &ref : lhs) {
    if (rhs.count(ref) == 0) {
      map.merge(ref.get());
    }
  }
  return map;
}
bool compare(const RefSet &lhs, const RefSet &rhs) {
  return diff_merge(lhs, rhs).includes(diff_merge(rhs, lhs));
}

}  // namespace

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
  return compare(heap_, env.heap_) && compare(stack_, env.stack_);
}

}  // namespace stacksafe
