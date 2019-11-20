#include "pool.hpp"
#include <algorithm>
#include "env.hpp"
#include "map.hpp"

namespace stacksafe {

MapPtr::MapPtr(const MultiMap& flat)
    : Super{std::make_unique<MultiMap>(flat)} {}
MapPtr::MapPtr(MapPtr&&) = default;
MapPtr::~MapPtr() = default;
MapPtr& MapPtr::operator=(MapPtr&&) = default;
const MultiMap& MapPtr::get() const {
  return *Super::get();
}
bool operator<(const MapPtr& lhs, const MapPtr& rhs) {
  return MultiMap::hash(lhs.get()) < MultiMap::hash(rhs.get());
}

MapRef MapPool::add(const MultiMap& flat) {
  MapPtr ptr{flat};
  const auto [lb, ub] = std::equal_range(begin(), end(), ptr);
  auto it = lb;
  for (; it != ub; ++it) {
    if (MultiMap::equals(it->get(), flat)) {
      return MapRef{it->get()};
    }
  }
  MapRef ref{ptr.get()};
  Super::insert(it, std::move(ptr));
  return ref;
}
Env MapPool::add(const FlatEnv& env) {
  auto heap = add(env.heap().to_multi());
  auto stack = add(env.stack().to_multi());
  return Env{heap, stack};
}

}  // namespace stacksafe
