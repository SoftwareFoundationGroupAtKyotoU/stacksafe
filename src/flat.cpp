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

void FlatMap::insert(const Value& key, const Symbol& val) {
  Super::emplace(key, val);
}
void FlatMap::insert(const Value& key, const Domain& val) {
  for (const auto& sym : val) {
    insert(key, sym);
  }
}
void FlatMap::merge(const FlatMap& flat) {
  for (const auto& to : flat) {
    Super::insert(to);
  }
}
bool FlatMap::includes(const FlatMap& flat) const {
  auto pred = [& self = *this](const auto& to) {
    return self.Super::count(to) != 0;
  };
  return std::all_of(flat.begin(), flat.end(), pred);
}
Map FlatMap::to_map(const FlatMap& flat) {
  Map map;
  for (const auto& to : flat) {
    map.insert(to.key(), to.val());
  }
  return map;
}
size_t FlatMap::hash(const FlatMap& flat) {
  std::size_t h = 0;
  for (const auto& to : flat) {
    h ^= MapsTo::hash(to);
  }
  return h;
}
bool FlatMap::equals(const FlatMap& lhs, const FlatMap& rhs) {
  return lhs.includes(rhs) && rhs.includes(lhs);
}
MultiMap FlatMap::to_multi() const {
  MultiMap m;
  for (const auto& to : *this) {
    m.insert(to.key(), to.val());
  }
  return m;
}

MapRef::MapRef(const MultiMap& flat) : flat_{&flat} {}
const MultiMap& MapRef::get() const {
  return *flat_;
}
bool operator==(const MapRef& lhs, const MapRef& rhs) {
  return MultiMap::equals(lhs.get(), rhs.get());
}

}  // namespace stacksafe
