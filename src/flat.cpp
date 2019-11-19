#include "flat.hpp"
#include "utility.hpp"

namespace std {
size_t hash<stacksafe::MapsTo>::operator()(const stacksafe::MapsTo& to) const {
  return stacksafe::MapsTo::hash(to);
}
}  // namespace std

namespace stacksafe {

MapsTo::MapsTo(const Value& val, const Symbol& sym) : Super{val, sym} {}
const Value& MapsTo::value() const {
  return std::get<0>(*this);
}
const Symbol& MapsTo::symbol() const {
  return std::get<1>(*this);
}
std::size_t MapsTo::hash(const MapsTo& to) {
  return hash_combine(Value::hash(to.value()), Symbol::hash(to.symbol()));
}
bool operator==(const MapsTo& lhs, const MapsTo& rhs) {
  return lhs.value() == rhs.value() && lhs.symbol() == rhs.symbol();
}

void FlatMap::insert(const Value& key, const Symbol& val) {
  Super::emplace(key, val);
}
size_t FlatMap::hash(const FlatMap& flat) {
  std::size_t h = 0;
  for (const auto& to : flat) {
    h ^= MapsTo::hash(to);
  }
  return h;
}

FlatMapPtr::FlatMapPtr(const FlatMap& flat)
    : Super{std::make_unique<FlatMap>(flat)} {}
const FlatMap& FlatMapPtr::get() const {
  return *Super::get();
}
bool operator<(const FlatMapPtr& lhs, const FlatMapPtr& rhs) {
  return FlatMap::hash(lhs.get()) < FlatMap::hash(lhs.get());
}

FlatMapRef::FlatMapRef(const FlatMap& flat) : flat_{&flat} {}
const FlatMap& FlatMapRef::get() const {
  return *flat_;
}

}  // namespace stacksafe
