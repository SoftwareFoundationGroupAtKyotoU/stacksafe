#include "pool.hpp"
#include <algorithm>
#include <iterator>
#include "hash.hpp"
#include "map.hpp"

namespace stacksafe {
namespace {
bool equals(MapRef lhs, MapRef rhs) {
  return &lhs.get() == &rhs.get();
}
bool compare(MapRef lhs, MapRef rhs) {
  return hash_value(lhs.get()) < hash_value(rhs.get());
}
}  // namespace

MapPtr::MapPtr(const Map &map) : Super{std::make_unique<Map>(map)} {}
MapPtr::MapPtr(MapPtr &&) = default;
MapPtr::~MapPtr() = default;
MapPtr &MapPtr::operator=(MapPtr &&) = default;
const Map &MapPtr::get() const {
  return *Super::get();
}
Map &MapPtr::get() {
  return *Super::get();
}

MapRef MapPool::add(const Map &map) {
  MapPtr ptr{map};
  const auto compare = [](const MapPtr &lhs, const MapPtr &rhs) {
    return hash_value(lhs.get()) < hash_value(rhs.get());
  };
  const auto [lb, ub] = std::equal_range(begin(), end(), ptr, compare);
  auto it = lb;
  for (; it != ub; ++it) {
    if (Map::equals(it->get(), map)) {
      return MapRef{it->get()};
    }
  }
  MapRef ref{ptr.get()};
  Super::insert(it, std::move(ptr));
  return ref;
}
void MapPool::remove(const MapRef &ref) {
  const auto pred = [&ref](const auto &ptr) {
    return &ptr.get() == &ref.get();
  };
  Super::erase(std::remove_if(begin(), end(), pred), end());
}

EnvOld::EnvOld(MapRef ref) : Super{ref} {}
Map EnvOld::concat() const {
  Map ret;
  for (const auto &r : *this) {
    ret.merge(r.get());
  }
  return ret;
}
void EnvOld::merge(const EnvOld &env) {
  auto len = size();
  Super::insert(end(), env.begin(), env.end());
  auto middle = std::next(begin(), len);
  std::inplace_merge(begin(), middle, end(), compare);
  erase(std::unique(begin(), end(), equals), end());
}
void EnvOld::insert(MapRef ref) {
  const auto [lb, ub] = std::equal_range(begin(), end(), ref, compare);
  if (lb == ub) {
    Super::insert(lb, ref);
  }
}
bool EnvOld::element(MapRef ref) const {
  const auto pred = [ref](const auto &map) { return equals(map, ref); };
  return std::find_if(begin(), end(), pred) != end();
}
bool EnvOld::element(const Value &key, const Value &val) const {
  const auto pred = [&key, &val](const auto &ref) {
    return ref.get().element(key, val);
  };
  return std::any_of(begin(), end(), pred);
}
bool EnvOld::includes(const EnvOld &env) const {
  const auto pred = [&self = *this](const auto &pair) {
    return self.element(pair.first, pair.second);
  };
  const auto pred2 = [&self = *this, pred](const auto &ref) {
    return (self.element(ref) ||
            std::all_of(ref.get().begin(), ref.get().end(), pred));
  };
  return std::all_of(env.begin(), env.end(), pred2);
}

}  // namespace stacksafe
