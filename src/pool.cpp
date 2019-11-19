#include "pool.hpp"
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
  MapRef ref{ptr.get()};
  Super::push_back(std::move(ptr));
  return ref;
}
Env MapPool::add(const FlatEnv& e) {
  auto heap = add(e.heap());
  auto stack = add(e.stack());
  return Env{heap, stack};
}

}  // namespace stacksafe
