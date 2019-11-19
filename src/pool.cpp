#include "pool.hpp"
#include <llvm/Support/raw_ostream.h>
#include <algorithm>
#include <set>
#include <string>
#include "env.hpp"
#include "map.hpp"
#include "utility.hpp"

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
  const auto [lb, ub] = std::equal_range(begin(), end(), ptr);
  auto it = lb;
  for (; it != ub; ++it) {
    if (it->get() == ptr.get()) {
      return MapRef{it->get()};
    } else {
      (llvm::errs() << "INFO: conflict\n").flush();
    }
  }
  MapRef ref{ptr.get()};
  Super::insert(it, std::move(ptr));
  return ref;
}
Env MapPool::add(const FlatEnv& e) {
  auto heap = add(e.heap());
  auto stack = add(e.stack());
  return Env{heap, stack};
}

}  // namespace stacksafe
