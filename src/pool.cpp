#include "pool.hpp"
#include <algorithm>
#include "env.hpp"
#include "map.hpp"

namespace stacksafe {

MapPtr::MapPtr(const Map& flat) : Super{std::make_unique<Map>(flat)} {}
MapPtr::MapPtr(MapPtr&&) = default;
MapPtr::~MapPtr() = default;
MapPtr& MapPtr::operator=(MapPtr&&) = default;
const Map& MapPtr::get() const {
  return *Super::get();
}
bool operator<(const MapPtr& lhs, const MapPtr& rhs) {
  return Map::hash(lhs.get()) < Map::hash(rhs.get());
}

MapRef MapPool::add(const Map& flat) {
  MapPtr ptr{flat};
  const auto [lb, ub] = std::equal_range(begin(), end(), ptr);
  auto it = lb;
  for (; it != ub; ++it) {
    if (Map::equals(it->get(), flat)) {
      return MapRef{it->get()};
    }
  }
  MapRef ref{ptr.get()};
  Super::insert(it, std::move(ptr));
  return ref;
}
Env MapPool::add(const FlatEnv& env) {
  auto heap = add(env.heap());
  auto stack = add(env.stack());
  return Env{heap, stack};
}

}  // namespace stacksafe
