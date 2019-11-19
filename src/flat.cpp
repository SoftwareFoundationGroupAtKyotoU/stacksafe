#include "flat.hpp"
#include <algorithm>
#include "domain.hpp"
#include "map.hpp"
#include "utility.hpp"

namespace std {
size_t hash<stacksafe::MapsTo>::operator()(const stacksafe::MapsTo& to) const {
  return stacksafe::MapsTo::hash(to);
}
size_t hash<stacksafe::FlatMapRef>::operator()(
    const stacksafe::FlatMapRef& f) const {
  return stacksafe::FlatMap::hash(f.get());
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

FlatMapRef::FlatMapRef(const FlatMap& flat) : flat_{&flat} {}
const FlatMap& FlatMapRef::get() const {
  return *flat_;
}
bool operator==(const FlatMapRef& lhs, const FlatMapRef& rhs) {
  return FlatMap::equals(lhs.get(), rhs.get());
}

}  // namespace stacksafe
