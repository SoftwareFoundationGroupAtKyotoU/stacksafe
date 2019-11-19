#include "pool.hpp"
#include <llvm/Support/raw_ostream.h>
#include <algorithm>
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
  const auto [lb, ub] = std::equal_range(begin(), end(), ptr);
  if (lb == ub) {
    Super::insert(lb, std::move(ptr));
  } else {
    bool dup = false;
    for (auto it = lb; it != ub; ++it) {
      if (it->get() == ptr.get()) {
        (llvm::errs() << "INFO: duplicate\n").flush();
        dup = true;
      } else {
        (llvm::errs() << "INFO: conflict\n").flush();
      }
    }
    if (!dup) {
      Super::insert(ub, std::move(ptr));
    }
  }
  return ref;
}
Env MapPool::add(const FlatEnv& e) {
  auto heap = add(e.heap());
  auto stack = add(e.stack());
  return Env{heap, stack};
}

}  // namespace stacksafe
