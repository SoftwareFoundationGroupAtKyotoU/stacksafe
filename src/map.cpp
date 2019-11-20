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
void MultiMap::insert(const Symbol &key, const Symbol &val) {
  insert(key.value(), val);
}
void MultiMap::insert(const Symbol &key, const Domain &val) {
  insert(key.value(), val);
}
Domain MultiMap::lookup(const Value &key) const {
  Domain dom;
  auto [lb, ub] = Super::equal_range(key);
  for (auto it = lb; it != ub; ++it) {
    dom.insert(it->second);
  }
  return dom;
}
Domain MultiMap::lookup(const Symbol &key) const {
  return lookup(key.value());
}
bool MultiMap::includes(const MultiMap &map) const {
  auto pred = [&self = *this](const auto &pair) {
    return self.find(pair.first) != self.end();
  };
  return std::all_of(map.begin(), map.end(), pred);
}
void MultiMap::merge(const MultiMap &map) {
  for (const auto &[key, val] : map) {
    insert(key, val);
  }
}
bool MultiMap::equals(const MultiMap &lhs, const MultiMap &rhs) {
  return lhs == rhs;
}
std::size_t MultiMap::hash(const MultiMap &map) {
  std::size_t ret = 0;
  for (const auto &[key, val] : map) {
    ret ^= hash_combine(Value::hash(key), Symbol::hash(val));
  }
  return ret;
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
MultiMap Map::to_multi() const {
  MultiMap m;
  for (const auto &[k, v] : *this) {
    m.insert(k, v);
  }
  return m;
}

}  // namespace stacksafe
