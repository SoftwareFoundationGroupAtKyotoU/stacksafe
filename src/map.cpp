#include "map.hpp"
#include <llvm/IR/Function.h>
#include <algorithm>

namespace stacksafe {

void Map::init(const llvm::Function &f) {
  const auto g = Value::get_symbol();
  get(g).insert(g);
  for (const auto &a : f.args()) {
    const auto arg = Value::get_symbol(a);
    get(arg).insert(arg);
    get(Value::get_register(a)).insert(arg);
  }
}
bool Map::insert(const Value &key, const Domain &dom) {
  return get(key).merge(dom);
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
    get(key).merge(val);
  }
}
Domain Map::keys(const Map &map) {
  Domain dom;
  for (const auto &pair : map) {
    dom.insert(pair.first);
  }
  return dom;
}
Domain &Map::get(const Value &key) {
  return Super::try_emplace(key).first->second;
}

}  // namespace stacksafe
