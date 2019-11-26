#include "map.hpp"
#include <llvm/IR/Function.h>
#include <algorithm>
#include "domain.hpp"

namespace stacksafe {

Map::Map() {}
Map::Map(std::size_t count) {}
Map::Map(std::size_t count, const llvm::Function &f) : Map{count} {
  const auto g = Value::get_symbol();
  insert(g, g);
  for (const auto &a : f.args()) {
    const auto arg = Value::get_symbol(a);
    insert(arg, arg);
    insert(Value::get_register(a), arg);
  }
}
bool Map::insert(const Value &key, const Value &val) {
  auto [lb, ub] = Super::equal_range(key);
  for (auto it = lb; it != ub; ++it) {
    if (it->second == val) {
      return false;
    }
  }
  Super::emplace_hint(lb, key, val);
  return true;
}
bool Map::insert(const Value &key, const Domain &dom) {
  bool diff = false;
  for (const auto &val : dom) {
    if (insert(key, val)) {
      diff = true;
    }
  }
  return diff;
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

}  // namespace stacksafe
