#include "flat.hpp"
#include <algorithm>
#include "domain.hpp"
#include "map.hpp"
#include "utility.hpp"

namespace std {
size_t hash<stacksafe::MapsTo>::operator()(const stacksafe::MapsTo& to) const {
  return stacksafe::MapsTo::hash(to);
}
size_t hash<stacksafe::MapRef>::operator()(const stacksafe::MapRef& f) const {
  return stacksafe::MultiMap::hash(f.get());
}
}  // namespace std

namespace stacksafe {

MapsTo::MapsTo(const Value& val, const Symbol& sym) : Super{val, sym} {}
const Value& MapsTo::key() const {
  return std::get<0>(*this);
}
const Symbol& MapsTo::val() const {
  return std::get<1>(*this);
}
std::size_t MapsTo::hash(const MapsTo& to) {
  return hash_combine(Value::hash(to.key()), Symbol::hash(to.val()));
}
bool operator==(const MapsTo& lhs, const MapsTo& rhs) {
  return lhs.key() == rhs.key() && lhs.val() == rhs.val();
}

MapRef::MapRef(const MultiMap& flat) : flat_{&flat} {}
const MultiMap& MapRef::get() const {
  return *flat_;
}
bool operator==(const MapRef& lhs, const MapRef& rhs) {
  return MultiMap::equals(lhs.get(), rhs.get());
}

}  // namespace stacksafe
