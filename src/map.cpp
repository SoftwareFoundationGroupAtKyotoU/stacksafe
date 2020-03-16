#include "map.hpp"

namespace stacksafe {

void Map::add(const Node& k, const Node& v) {
  const auto t = RedBlackTree::single(std::make_tuple(k, v));
  tree_ = RedBlackTree::merge(tree_, t);
}

}  // namespace stacksafe
