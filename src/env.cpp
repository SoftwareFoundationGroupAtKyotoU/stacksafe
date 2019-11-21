#include "env.hpp"
#include "utility.hpp"

namespace stacksafe {

Env::Env(MapRef ref) : set_{ref} {}
Map Env::concat() const {
  Map ret;
  for (const auto &r : set_) {
    ret.merge(r.get());
  }
  return ret;
}
void Env::merge(const Env &env) {
  set_.insert(env.set_.begin(), env.set_.end());
}
void Env::insert(MapRef ref) {
  if (!concat().includes(ref.get())) {
    set_.emplace(ref);
  }
}
bool Env::includes(const Env &env) {
  return concat().includes(env.concat());
}

}  // namespace stacksafe
