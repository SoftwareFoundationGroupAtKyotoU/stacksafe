#include "map.hpp"
#include <llvm/IR/Function.h>
#include <algorithm>

namespace stacksafe {

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
  auto it = Super::try_emplace(key, Domain{}).first;
  return it->second.insert(val);
}
bool Map::insert(const Value &key, const Domain &dom) {
  auto it = Super::try_emplace(key, Domain{}).first;
  return it->second.merge(dom);
}
Domain Map::lookup(const Value &key) const {
  if (auto it = Super::find(key); it != end()) {
    return it->second;
  } else {
    return Domain{};
  }
}
void Map::merge(const Map &map) {
  for (const auto &[key, val] : map) {
    insert(key, val);
  }
}
Domain Map::keys(const Map &map) {
  Domain dom;
  for (const auto &[key, val] : map) {
    dom.insert(key);
  }
  return dom;
}

}  // namespace stacksafe
