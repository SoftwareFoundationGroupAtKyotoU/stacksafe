#include "map.hpp"
#include "graph.hpp"

namespace stacksafe {

void Map::add(const Node& k, const Node& v) {
  const auto t = Tree::single(std::make_tuple(k, v));
  tree_ = Tree::merge(tree_, t);
}
bool Map::exists(const Node& k, const Node& v) const {
  return exists(tree_, std::make_tuple(k, v));
}
NodeSet Map::lookup(const Node& k) const {
  return Tree::find(tree_, k);
}
void Map::merge(const Map& m) {
  tree_ = merge(tree_, m.tree_);
}
std::size_t Map::size() const {
  return Tree::get_size(tree_);
}
TreePtr Map::merge(const TreePtr& x, const TreePtr& y) {
  if (Tree::get_size(x) < Tree::get_size(y)) {
    return merge(y, x);
  } else if (x && y) {
    const auto [l, b, r] = Tree::split(x, y->value());
    return Tree::join(merge(l, y->left()), y->value(), merge(r, y->right()));
  } else {
    return x ? x : y;
  }
}
bool Map::exists(const TreePtr& x, const PairType& v) {
  if (!x) {
    return false;
  } else if (x->value() < v) {
    return exists(x->right(), v);
  } else if (v < x->value()) {
    return exists(x->left(), v);
  } else {
    return true;
  }
}

}  // namespace stacksafe
