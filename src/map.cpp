#include "map.hpp"
#include "utility.hpp"

namespace stacksafe {

void Map::insert(const Value &key, const Domain &val) {
  auto [it, updated] = Super::try_emplace(key, val);
  if (!updated) {
    it->second.merge(val);
  }
}
void Map::insert(const Value &key, const Symbol &val) {
  Super::try_emplace(key, Domain{}).first->second.insert(val);
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
