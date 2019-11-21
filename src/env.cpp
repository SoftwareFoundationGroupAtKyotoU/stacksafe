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

Env::Env(MapRef ref) : set_{ref} {}
Map Env::heap() const {
  Map ret;
  for (const auto &r : set_) {
    ret.merge(r.get());
  }
  return ret;
}
Map Env::stack() const {
  Map ret;
  for (const auto &r : set_) {
    ret.merge(r.get());
  }
  return ret;
}
void Env::merge(const Env &env) {
  set_.insert(env.set_.begin(), env.set_.end());
}
void Env::merge_heap(MapRef ref) {
  set_.emplace(ref);
}
void Env::merge_stack(MapRef ref) {
  set_.emplace(ref);
}
bool Env::includes(const Env &env) {
  return compare(set_, env.set_);
}

}  // namespace stacksafe
