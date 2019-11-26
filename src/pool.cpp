#include "pool.hpp"
#include <algorithm>
#include <iterator>
#include "hash.hpp"
#include "map.hpp"

namespace stacksafe {

MapPtr::MapPtr(const Map &map) : Super{std::make_unique<Map>(map)} {}
MapPtr::MapPtr(MapPtr &&) = default;
MapPtr::~MapPtr() = default;
const Map &MapPtr::get() const {
  return *Super::get();
}
bool operator==(const MapPtr &lhs, const MapPtr &rhs) {
  return hash_value(lhs.get()) == hash_value(rhs.get());
}

}  // namespace stacksafe
