#include "map.hpp"
#include "graph.hpp"

namespace stacksafe {

void Map::add(const Node& k, const Node& v) {
  tree_ = merge(tree_, TreeType::make(k, v));
}
bool Map::exists(const Node& k, const Node& v) const {
  return exists(tree_, std::make_tuple(k, v));
}
NodeSet Map::lookup(const Node& k) const {
  NodeSet nodes;
  lookup(tree_, k, nodes);
  return nodes;
}
void Map::merge(const Map& m) {
  tree_ = merge(tree_, m.tree_);
}
std::size_t Map::size() const {
  return TreeType::size(tree_);
}
auto Map::merge(const TreePtr& x, const TreePtr& y) -> TreePtr {
  if (TreeType::size(x) < TreeType::size(y)) {
    return merge(y, x);
  } else if (x && y) {
    const auto [l, b, r] = TreeType::split(x, y->value());
    return TreeType::join(merge(l, y->left()), y->value(),
                          merge(r, y->right()));
  } else {
    return x ? x : y;
  }
}
void Map::lookup(const TreePtr& x, const Node& key, NodeSet& nodes) {
  if (x) {
    const auto [k, v] = x->value();
    if (k < key) {
      lookup(x->right(), key, nodes);
    } else if (key < k) {
      lookup(x->left(), key, nodes);
    } else {
      lookup(x->left(), key, nodes);
      nodes.merge(NodeSet{v});
      lookup(x->right(), key, nodes);
    }
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
