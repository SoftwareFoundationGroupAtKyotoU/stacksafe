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

MapRef MapPool::add(const Map& m) {
  MapPtr ptr{m};
  const auto [lb, ub] = std::equal_range(begin(), end(), ptr);
  auto it = lb;
  for (; it != ub; ++it) {
    if (it->get() == m) {
      return MapRef{it->get()};
    }
  }
  MapRef ref{ptr.get()};
  Super::insert(it, std::move(ptr));
  return ref;
}
EnvOld MapPool::add(const FlatEnvOld& e) {
  auto heap = add(e.heap());
  auto stack = add(e.stack());
  return EnvOld{heap, stack};
}

}  // namespace stacksafe
