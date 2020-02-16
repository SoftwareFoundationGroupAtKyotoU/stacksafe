#include "map.hpp"
#include <llvm/IR/Function.h>

namespace stacksafe {

void Map::init(const llvm::Function &f) {
  const auto g = Symbol::get_global();
  get(Node{g}).insert(g);
  for (const auto &a : f.args()) {
    const Symbol sym{a};
    const Register reg{a};
    get(Node{sym}).insert(sym);
    get(Node{reg}).insert(sym);
  }
}
bool Map::insert(const Symbol &key, const Domain &dom) {
  return get(Node{key}).merge(dom);
}
bool Map::insert(const Register &key, const Domain &dom) {
  return get(Node{key}).merge(dom);
}
Domain Map::lookup(const Symbol &key) const {
  return find(Node{key});
}
Domain Map::lookup(const Register &key) const {
  return find(Node{key});
}
void Map::merge(const Map &map) {
  for (const auto &[key, val] : map) {
    get(key).merge(val);
  }
}
std::set<Node> Map::keys(const Map &map) {
  std::set<Node> dom;
  for (const auto &pair : map) {
    dom.emplace(pair.first);
  }
  return dom;
}
Domain Map::find(const Node &key) const {
  if (auto it = Super::find(key); it != end()) {
    return it->second;
  } else {
    return Domain{};
  }
}
Domain &Map::get(const Node &key) {
  return Super::try_emplace(key).first->second;
}

}  // namespace stacksafe
