#include "map.hpp"
#include <llvm/IR/Function.h>
#include <algorithm>
#include "domain.hpp"

namespace stacksafe {
namespace {
bool compare(const Pair &lhs, const Pair &rhs) {
  return lhs.key() < rhs.key();
}
}  // namespace

Map::Map(const llvm::Function &f) {
  const auto g = Value::get_symbol();
  insert(g, g);
  for (const auto &a : f.args()) {
    const auto arg = Value::get_symbol(a);
    insert(arg, arg);
    insert(Value::get_register(a), arg);
  }
}
bool Map::insert(const Value &key, const Value &val) {
  return Super::emplace(key, val).second;
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
  const auto [lb, ub] =
      std::equal_range(begin(), end(), Pair{key, key}, compare);
  Domain dom;
  for (auto it = lb; it != ub; ++it) {
    dom.insert(it->val());
  }
  return dom;
}
void Map::merge(const Map &map) {
  for (const auto &pair : map) {
    insert(pair.key(), pair.val());
  }
}
Domain Map::keys(const Map &map) {
  Domain dom;
  for (const auto &pair : map) {
    dom.insert(pair.key());
  }
  return dom;
}

}  // namespace stacksafe
