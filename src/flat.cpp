#include "flat.hpp"
#include <algorithm>
#include "domain.hpp"
#include "map.hpp"
#include "utility.hpp"

namespace std {
size_t hash<stacksafe::MapRef>::operator()(const stacksafe::MapRef& f) const {
  return stacksafe::MultiMap::hash(f.get());
}
}  // namespace std

namespace stacksafe {

MapRef::MapRef(const MultiMap& flat) : flat_{&flat} {}
const MultiMap& MapRef::get() const {
  return *flat_;
}
bool operator==(const MapRef& lhs, const MapRef& rhs) {
  return MultiMap::equals(lhs.get(), rhs.get());
}

}  // namespace stacksafe
