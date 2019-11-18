#include "pool.hpp"
#include "env.hpp"
#include "map.hpp"

namespace stacksafe {

MapRef::MapRef(const Map& m) : map_{&m} {}
const Map& MapRef::get() const {
  return *map_;
}

MapRef MapPool::add(const Map& m) {
  auto ptr = std::make_unique<Map>(m);
  MapRef ref{*ptr.get()};
  Super::push_back(std::move(ptr));
  return ref;
}
Env MapPool::add(const FlatEnv& e) {
  Env ret;
  auto heap = add(e.heap());
  auto stack = add(e.stack());
  ret.merge(FlatEnv{heap.get(), stack.get()});
  return ret;
}

}  // namespace stacksafe
