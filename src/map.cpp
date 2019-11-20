#include "map.hpp"
#include "utility.hpp"

namespace stacksafe {

void MultiMap::insert(const Value &key, const Symbol &val) {
  auto [lb, ub] = Super::equal_range(key);
  for (auto it = lb; it != ub; ++it) {
    if (it->second == val) {
      return;
    }
  }
  Super::emplace_hint(lb, key, val);
}
void MultiMap::insert(const Value &key, const Domain &val) {
  for (const auto &sym : val) {
    insert(key, sym);
  }
}
Domain MultiMap::lookup(const Value &key) const {
  Domain dom;
  auto [lb, ub] = Super::equal_range(key);
  for (auto it = lb; it != ub; ++it) {
    dom.insert(it->second);
  }
  return dom;
}
void MultiMap::merge(const MultiMap &map) {
  for (const auto &[k, v] : map) {
    insert(k, v);
  }
}

void Map::insert(const Symbol &key, const Domain &val) {
  insert(key.value(), val);
}
void Map::insert(const Value &key, const Domain &val) {
  auto [it, updated] = Super::try_emplace(key, val);
  if (!updated) {
    it->second.merge(val);
  }
}
void Map::insert(const Value &key, const Symbol &val) {
  Super::try_emplace(key, Domain{}).first->second.insert(val);
}
Domain Map::lookup(const Symbol &key) const {
  return lookup(key.value());
}
Domain Map::lookup(const Value &key) const {
  if (auto it = Super::find(key); it != end()) {
    return it->second;
  }
  return Domain{};
}
void Map::merge(const Map &that) {
  for (const auto &[k, v] : that) {
    insert(k, v);
  }
}

Heap::Heap(const Map &m) : Map{m} {}
void Heap::insert(const Symbol &key, const Domain &val) {
  Map::insert(key.value(), val);
}
Domain Heap::lookup(const Symbol &key) const {
  return Map::lookup(key.value());
}
void Heap::merge(const Heap &that) {
  Map::merge(that);
}
Stack::Stack(const Map &m) : Map{m} {}

}  // namespace stacksafe
