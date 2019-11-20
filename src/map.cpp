#include "map.hpp"
#include <llvm/IR/Function.h>
#include "domain.hpp"
#include "utility.hpp"

namespace stacksafe {

void Map::insert(const Value &key, const Symbol &val) {
  auto [lb, ub] = Super::equal_range(key);
  for (auto it = lb; it != ub; ++it) {
    if (it->second == val) {
      return;
    }
  }
  Super::emplace_hint(lb, key, val);
}
void Map::insert(const Value &key, const Domain &val) {
  for (const auto &sym : val) {
    insert(key, sym);
  }
}
void Map::insert(const Symbol &key, const Symbol &val) {
  insert(key.value(), val);
}
void Map::insert(const Symbol &key, const Domain &val) {
  insert(key.value(), val);
}
Domain Map::lookup(const Value &key) const {
  Domain dom;
  auto [lb, ub] = Super::equal_range(key);
  for (auto it = lb; it != ub; ++it) {
    dom.insert(it->second);
  }
  return dom;
}
Domain Map::lookup(const Symbol &key) const {
  return lookup(key.value());
}
bool Map::includes(const Map &map) const {
  auto pred = [&self = *this](const auto &pair) {
    return self.find(pair.first) != self.end();
  };
  return std::all_of(map.begin(), map.end(), pred);
}
void Map::merge(const Map &map) {
  for (const auto &[key, val] : map) {
    insert(key, val);
  }
}
Map Map::init_heap(const llvm::Function &f) {
  Map heap;
  const auto g = Symbol::get_global();
  heap.insert(g, g);
  for (const auto &a : f.args()) {
    const auto arg = Symbol::get_arg(a);
    heap.insert(arg, arg);
  }
  return heap;
}
Map Map::init_stack(const llvm::Function &f) {
  Map stack;
  for (const auto &a : f.args()) {
    const auto arg = Symbol::get_arg(a);
    stack.insert(a, arg);
  }
  return stack;
}
bool Map::equals(const Map &lhs, const Map &rhs) {
  return lhs == rhs;
}
std::unordered_set<Value> Map::keys(const Map &map) {
  std::unordered_set<Value> vec;
  for (const auto &pair : map) {
    vec.emplace(pair.first);
  }
  return vec;
}
std::size_t Map::hash(const Map &map) {
  std::size_t ret = 0;
  for (const auto &[key, val] : map) {
    ret ^= hash_combine(Value::hash(key), Symbol::hash(val));
  }
  return ret;
}

MapRef::MapRef(const Map &flat) : flat_{&flat} {}
const Map &MapRef::get() const {
  return *flat_;
}
bool operator==(const MapRef &lhs, const MapRef &rhs) {
  return Map::equals(lhs.get(), rhs.get());
}

}  // namespace stacksafe

namespace std {
size_t hash<stacksafe::MapRef>::operator()(const stacksafe::MapRef &f) const {
  return stacksafe::Map::hash(f.get());
}
}  // namespace std
