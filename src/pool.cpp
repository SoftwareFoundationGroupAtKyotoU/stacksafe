#include "pool.hpp"
#include <algorithm>
#include <iterator>
#include "env.hpp"
#include "hash.hpp"
#include "map.hpp"

namespace stacksafe {

MapPtr::MapPtr(const Map &map) : Super{std::make_unique<Map>(map)} {}
MapPtr::MapPtr(MapPtr &&) = default;
MapPtr::~MapPtr() = default;
const Map &MapPtr::get() const {
  return *Super::get();
}
bool operator==(const MapPtr &lhs, const MapPtr &rhs) {
  return hash_value(lhs.get()) == hash_value(rhs.get());
}

MapPool::MapPool(std::size_t count) : count_{count} {}
MapRef MapPool::add(const Map &map) {
  MapPtr ptr{map};
  const auto [lb, ub] = Super::equal_range(ptr);
  for (auto it = lb; it != ub; ++it) {
    if (it->get().equals(map)) {
      return MapRef{it->get()};
    }
  }
  MapRef ref{ptr.get()};
  Super::emplace_hint(ub, std::move(ptr));
  return ref;
}
Map MapPool::make_map() const {
  return Map{};
}
Env MapPool::make_env() const {
  return Env{count_};
}
Env MapPool::init(const llvm::Function &f) {
  Env env{count_};
  Map map{f};
  if (!map.empty()) {
    env.insert(add(map));
  }
  return env;
}

}  // namespace stacksafe
