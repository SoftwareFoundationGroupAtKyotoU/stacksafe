#include "map.hpp"
#include <llvm/IR/Function.h>
#include <algorithm>
#include "domain.hpp"
#include "utility.hpp"

namespace stacksafe {

Map::Map() : hash_{0} {}
void Map::insert(const Value &key, const Value &val) {
  auto [lb, ub] = Super::equal_range(key);
  for (auto it = lb; it != ub; ++it) {
    if (it->second == val) {
      return;
    }
  }
  Super::emplace_hint(lb, key, val);
  hash_ ^= llvm::hash_combine(key, val);
}
void Map::insert(const Value &key, const Domain &val) {
  for (const auto &sym : val) {
    insert(key, sym);
  }
}
Domain Map::lookup(const Value &key) const {
  Domain dom;
  auto [lb, ub] = Super::equal_range(key);
  for (auto it = lb; it != ub; ++it) {
    dom.insert(it->second);
  }
  return dom;
}
bool Map::element(const Value &key, const Value &val) const {
  const auto pred = [&val](const auto &pair) { return pair.second == val; };
  auto [lb, ub] = Super::equal_range(key);
  return std::any_of(lb, ub, pred);
}
bool Map::includes(const Map &map) const {
  auto pred = [&self = *this](const auto &pair) {
    return self.find(pair.first) != self.end();
  };
  return std::all_of(map.begin(), map.end(), pred);
}
void Map::merge(const Map &map) {
  Super::insert(map.begin(), map.end());
}
Map Map::init(const llvm::Function &f) {
  Map map;
  const auto g = Value::get_symbol();
  map.insert(g, g);
  for (const auto &a : f.args()) {
    const auto arg = Value::get_symbol(a);
    map.insert(arg, arg);
    map.insert(Value::get_register(a), arg);
  }
  return map;
}
bool Map::equals(const Map &lhs, const Map &rhs) {
  return lhs == rhs;
}
Domain Map::keys(const Map &map) {
  Domain dom;
  for (const auto &pair : map) {
    dom.insert(pair.first);
  }
  return dom;
}

MapRef::MapRef(const Map &map) : ptr_{&map} {}
const Map &MapRef::get() const {
  return *ptr_;
}

}  // namespace stacksafe
