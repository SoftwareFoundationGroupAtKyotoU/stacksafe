#include "env.hpp"
#include <algorithm>
#include <iterator>
#include "utility.hpp"

namespace stacksafe {
namespace {
bool equals(MapRef lhs, MapRef rhs) {
  return &lhs.get() == &rhs.get();
}
bool compare(MapRef lhs, MapRef rhs) {
  return hash_value(lhs.get()) < hash_value(rhs.get());
}
}  // namespace

Env::Env(MapRef ref) : Super{ref} {}
Map Env::concat() const {
  Map ret;
  for (const auto &r : *this) {
    ret.merge(r.get());
  }
  return ret;
}
void Env::merge(const Env &env) {
  auto len = size();
  Super::insert(end(), env.begin(), env.end());
  auto middle = std::next(begin(), len);
  std::inplace_merge(begin(), middle, end(), compare);
  erase(std::unique(begin(), end(), equals), end());
}
void Env::insert(MapRef ref) {
  const auto [lb, ub] = std::equal_range(begin(), end(), ref, compare);
  if (lb == ub) {
    Super::insert(lb, ref);
  }
}
bool Env::includes(const Env &env) {
  return concat().includes(env.concat());
}

}  // namespace stacksafe
