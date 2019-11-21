#include "env.hpp"
#include "utility.hpp"

namespace stacksafe {

Env::Env(MapRef ref) : Super{ref} {}
Map Env::concat() const {
  Map ret;
  for (const auto &r : *this) {
    ret.merge(r.get());
  }
  return ret;
}
void Env::merge(const Env &env) {
  Super::insert(env.begin(), env.end());
}
void Env::insert(MapRef ref) {
  if (!concat().includes(ref.get())) {
    Super::emplace(ref);
  }
}
bool Env::includes(const Env &env) {
  return concat().includes(env.concat());
}

}  // namespace stacksafe
