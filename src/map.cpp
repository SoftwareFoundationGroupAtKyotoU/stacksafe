#include "map.hpp"
#include <llvm/IR/Function.h>
#include <algorithm>

namespace stacksafe {

void Map::init(const llvm::Function &f) {
  const auto g = Symbol::get_global();
  get(g).insert(g);
  for (const auto &a : f.args()) {
    const Symbol sym{a};
    const Register reg{a};
    get(sym).insert(sym);
    get(reg).insert(sym);
  }
}
bool Map::insert(const Symbol &key, const Domain &dom) {
  return get(key).merge(dom);
}
bool Map::insert(const Register &key, const Domain &dom) {
  return get(key).merge(dom);
}
Domain Map::lookup(const Symbol &key) const {
  return find(Value{key});
}
Domain Map::lookup(const Register &key) const {
  return find(Value{key});
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
Domain Map::find(const Value &key) const {
  if (auto it = Super::find(key); it != end()) {
    return it->second;
  } else {
    return Domain{};
  }
}
Domain &Map::get(const Value &key) {
  return Super::try_emplace(key).first->second;
}
Domain &Map::get(const Symbol &key) {
  return get(Value{key});
}
Domain &Map::get(const Register &key) {
  return get(Value{key});
}

}  // namespace stacksafe
