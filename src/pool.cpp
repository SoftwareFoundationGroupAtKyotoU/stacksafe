#include "pool.hpp"
#include <algorithm>
#include "env.hpp"
#include "map.hpp"

namespace stacksafe {

FlatMapPtr::FlatMapPtr(const FlatMap& flat)
    : Super{std::make_unique<MultiMap>(flat.to_multi())} {}
FlatMapPtr::FlatMapPtr(FlatMapPtr&&) = default;
FlatMapPtr::~FlatMapPtr() = default;
FlatMapPtr& FlatMapPtr::operator=(FlatMapPtr&&) = default;
const MultiMap& FlatMapPtr::get() const {
  return *Super::get();
}
bool operator<(const FlatMapPtr& lhs, const FlatMapPtr& rhs) {
  return MultiMap::hash(lhs.get()) < MultiMap::hash(rhs.get());
}

FlatMapRef FlatMapPool::add(const FlatMap& flat) {
  FlatMapPtr ptr{flat};
  const auto [lb, ub] = std::equal_range(begin(), end(), ptr);
  auto it = lb;
  for (; it != ub; ++it) {
    if (MultiMap::equals(it->get(), flat.to_multi())) {
      return FlatMapRef{it->get()};
    }
  }
  FlatMapRef ref{ptr.get()};
  Super::insert(it, std::move(ptr));
  return ref;
}
Env FlatMapPool::add(const FlatEnv& env) {
  auto heap = add(env.heap());
  auto stack = add(env.stack());
  return Env{heap, stack};
}

}  // namespace stacksafe
