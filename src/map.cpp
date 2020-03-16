#include "map.hpp"
#include "graph.hpp"

namespace stacksafe {

void Map::add(const Node& k, const Node& v) {
  const auto t = RedBlackTree::single(std::make_tuple(k, v));
  tree_ = RedBlackTree::merge(tree_, t);
}
bool Map::exists(const Node& k, const Node& v) const {
  return RedBlackTree::exists(tree_, std::make_tuple(k, v));
}
NodeSet Map::lookup(const Node& k) const {
  return RedBlackTree::find(tree_, k);
}

}  // namespace stacksafe
