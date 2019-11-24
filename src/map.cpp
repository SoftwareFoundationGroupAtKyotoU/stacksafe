#include "map.hpp"
#include <llvm/IR/Function.h>
#include <algorithm>
#include "domain.hpp"

namespace stacksafe {

Map::Map(std::size_t count) : hash_{0} {}
Map::Map(std::size_t count, const llvm::Function &f) : Map{count} {
  const auto g = Value::get_symbol();
  insert(g, g);
  for (const auto &a : f.args()) {
    const auto arg = Value::get_symbol(a);
    insert(arg, arg);
    insert(Value::get_register(a), arg);
  }
}
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
Domain Map::lookup(const Value &key) const {
  Domain dom;
  auto [lb, ub] = Super::equal_range(key);
  for (auto it = lb; it != ub; ++it) {
    dom.insert(it->second);
  }
  return dom;
}
bool Map::contains(const Value &key, const Value &val) const {
  const auto pred = [&val](const auto &pair) { return pair.second == val; };
  auto [lb, ub] = Super::equal_range(key);
  return std::any_of(lb, ub, pred);
}
bool Map::equals(const Map &map) const {
  const Super &lhs = *this;
  const Super &rhs = map;
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
