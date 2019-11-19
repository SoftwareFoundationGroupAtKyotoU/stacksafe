#include "flat.hpp"
#include <algorithm>
#include "domain.hpp"
#include "env.hpp"
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
void FlatMap::insert(const Value& key, const Domain& val) {
  for (const auto& sym : val) {
    insert(key, sym);
  }
}
void FlatMap::insert(const Map& map) {
  for (const auto& [key, val] : map) {
    insert(key, val);
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

FlatMapPtr::FlatMapPtr(const FlatMap& flat)
    : Super{std::make_unique<FlatMap>(flat)} {}
const FlatMap& FlatMapPtr::get() const {
  return *Super::get();
}
bool operator<(const FlatMapPtr& lhs, const FlatMapPtr& rhs) {
  return FlatMap::hash(lhs.get()) < FlatMap::hash(rhs.get());
}

FlatMapRef::FlatMapRef(const FlatMap& flat) : flat_{&flat} {}
const FlatMap& FlatMapRef::get() const {
  return *flat_;
}
bool operator==(const FlatMapRef& lhs, const FlatMapRef& rhs) {
  return FlatMap::equals(lhs.get(), rhs.get());
}

FlatMapRef FlatMapPool::add(const FlatMap& flat) {
  FlatMapPtr ptr{flat};
  const auto [lb, ub] = std::equal_range(begin(), end(), ptr);
  auto it = lb;
  for (; it != ub; ++it) {
    if (FlatMap::equals(it->get(), flat)) {
      return FlatMapRef{it->get()};
    }
  }
  FlatMapRef ref{it->get()};
  Super::insert(it, std::move(ptr));
  return ref;
}
Env FlatMapPool::add(const FlatEnv& env) {
  auto heap = add(env.heap());
  auto stack = add(env.stack());
  return Env{heap, stack};
}

}  // namespace stacksafe
