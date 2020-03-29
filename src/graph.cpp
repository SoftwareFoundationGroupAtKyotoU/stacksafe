#include "graph.hpp"

namespace stacksafe {

void Graph::merge(const Graph& g) {
  tree_ = merge(tree_, g.tree_);
}
auto Graph::merge(const TreePtr& x, const TreePtr& y) -> TreePtr {
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

}  // namespace stacksafe
