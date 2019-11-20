#include "pool.hpp"
#include <algorithm>
#include "env.hpp"
#include "map.hpp"

namespace stacksafe {

MapPtr::MapPtr(const FlatMap& flat)
    : Super{std::make_unique<MultiMap>(flat.to_multi())} {}
MapPtr::MapPtr(MapPtr&&) = default;
MapPtr::~MapPtr() = default;
MapPtr& MapPtr::operator=(MapPtr&&) = default;
const MultiMap& MapPtr::get() const {
  return *Super::get();
}
bool operator<(const MapPtr& lhs, const MapPtr& rhs) {
  return MultiMap::hash(lhs.get()) < MultiMap::hash(rhs.get());
}

FlatMapRef FlatMapPool::add(const FlatMap& flat) {
  MapPtr ptr{flat};
  const auto [lb, ub] = std::equal_range(begin(), end(), ptr);
  auto it = lb;
  for (; it != ub; ++it) {
    if (MultiMap::equals(it->get(), flat.to_multi())) {
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
