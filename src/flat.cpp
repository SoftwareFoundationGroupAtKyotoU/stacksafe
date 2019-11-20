#include "flat.hpp"
#include <algorithm>
#include "domain.hpp"
#include "map.hpp"
#include "utility.hpp"

namespace std {
size_t hash<stacksafe::MapRef>::operator()(const stacksafe::MapRef& f) const {
  return stacksafe::Map::hash(f.get());
}
}  // namespace std

namespace stacksafe {

MapRef::MapRef(const Map& flat) : flat_{&flat} {}
const Map& MapRef::get() const {
  return *flat_;
}
bool operator==(const MapRef& lhs, const MapRef& rhs) {
  return Map::equals(lhs.get(), rhs.get());
}

}  // namespace stacksafe
