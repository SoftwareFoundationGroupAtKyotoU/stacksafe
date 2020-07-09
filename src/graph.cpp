#include "graph.hpp"

namespace stacksafe {

void Graph::add(const Node &key, const Node &val) {
  tree_ = merge(tree_, TreeType::make(key, val));
}
void Graph::merge(const Graph &g) {
  tree_ = merge(tree_, g.tree_);
}
NodeSet Graph::lookup(const Node &n) const {
  NodeSet nodes;
  lookup(tree_, n, nodes);
  return nodes;
}
bool Graph::exists(const Node &key, const Node &val) const {
  return exists(tree_, std::make_tuple(key, val));
}
std::size_t Graph::size() const {
  return TreeType::size(tree_);
}
auto Graph::merge(const TreePtr &x, const TreePtr &y) -> TreePtr {
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
void Graph::lookup(const TreePtr &x, const Node &key, NodeSet &nodes) {
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
bool Graph::exists(const TreePtr &x, const PairType &v) {
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
