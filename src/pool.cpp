#include "pool.hpp"
#include <algorithm>
#include "env.hpp"
#include "map.hpp"

namespace stacksafe {

MapPtr::MapPtr(const Map& m) : Super{std::make_unique<Map>(m)} {}
const Map& MapPtr::get() const {
  return *Super::get();
}
bool operator<(const MapPtr& lhs, const MapPtr& rhs) {
  return Map::hash(lhs.get()) < Map::hash(rhs.get());
}

FlatMapPtr::FlatMapPtr(const FlatMap& flat)
    : Super{std::make_unique<FlatMap>(flat)} {}
const FlatMap& FlatMapPtr::get() const {
  return *Super::get();
}
bool operator<(const FlatMapPtr& lhs, const FlatMapPtr& rhs) {
  return FlatMap::hash(lhs.get()) < FlatMap::hash(rhs.get());
}

FlatMapRef FlatMapPool::add(const FlatMap& flat) {
  FlatMapPtr ptr{flat};
  const auto [lb, ub] = std::equal_range(begin(), end(), ptr);
  auto it = lb;
  for (; it != ub; ++it) {
    if (FlatMap::equals(it->get(), flat)) {
      return FlatMapRef{it->get()};
    }
  }
  FlatMapRef ref{ptr.get()};
  Super::insert(it, std::move(ptr));
  return ref;
}
Env FlatMapPool::add(const FlatEnv& env) {
  auto heap = add(env.heap());
  auto stack = add(env.stack());
  return Env{heap, stack};
}

}  // namespace stacksafe
