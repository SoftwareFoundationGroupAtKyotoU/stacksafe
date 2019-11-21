#include "pool.hpp"
#include <algorithm>
#include "hash.hpp"
#include "map.hpp"

namespace stacksafe {

MapPtr::MapPtr(const Map& map) : Super{std::make_unique<Map>(map)} {}
MapPtr::MapPtr(MapPtr&&) = default;
MapPtr::~MapPtr() = default;
MapPtr& MapPtr::operator=(MapPtr&&) = default;
const Map& MapPtr::get() const {
  return *Super::get();
}

MapRef MapPool::add(const Map& map) {
  MapPtr ptr{map};
  const auto compare = [](const MapPtr& lhs, const MapPtr& rhs) {
    return hash_value(lhs.get()) < hash_value(rhs.get());
  };
  const auto [lb, ub] = std::equal_range(begin(), end(), ptr, compare);
  auto it = lb;
  for (; it != ub; ++it) {
    if (Map::equals(it->get(), map)) {
      return MapRef{it->get()};
    }
  }
  MapRef ref{ptr.get()};
  Super::insert(it, std::move(ptr));
  return ref;
}

}  // namespace stacksafe
