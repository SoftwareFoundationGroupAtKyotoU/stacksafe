#include "map.hpp"
#include <llvm/IR/Function.h>
#include <algorithm>

namespace stacksafe {

void Map::init(const llvm::Function &f) {
  const auto g = Symbol::get_global();
  get(Value{g}).insert(g);
  for (const auto &a : f.args()) {
    const Symbol sym{a};
    const Register reg{a};
    get(Value{sym}).insert(sym);
    get(Value{reg}).insert(sym);
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
std::set<Value> Map::keys(const Map &map) {
  std::set<Value> dom;
  for (const auto &pair : map) {
    dom.emplace(pair.first);
  }
  return dom;
}
Domain &Map::get(const Value &key) {
  return Super::try_emplace(key).first->second;
}

}  // namespace stacksafe
