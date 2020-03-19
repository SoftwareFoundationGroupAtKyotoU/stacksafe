#include "map.hpp"
#include "graph.hpp"

namespace stacksafe {

void Map::add(const Node& k, const Node& v) {
  const auto t = Tree::single(std::make_tuple(k, v));
  tree_ = Tree::merge(tree_, t);
}
bool Map::exists(const Node& k, const Node& v) const {
  return Tree::exists(tree_, std::make_tuple(k, v));
}
NodeSet Map::lookup(const Node& k) const {
  return Tree::find(tree_, k);
}
void Map::merge(const Map& m) {
  tree_ = Tree::merge(tree_, m.tree_);
}
std::size_t Map::size() const {
  return Tree::get_size(tree_);
}

}  // namespace stacksafe
