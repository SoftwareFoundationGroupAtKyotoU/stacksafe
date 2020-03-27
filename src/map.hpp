#ifndef INCLUDE_GUARD_B24913BE_A7FB_43A0_9D42_907C04FE896D
#define INCLUDE_GUARD_B24913BE_A7FB_43A0_9D42_907C04FE896D

#include "node.hpp"
#include "tree.hpp"

namespace stacksafe {
class NodeSet;

template <typename Key>
class Map {
  using TreeType = Tree<Key, Node>;
  using PairType = typename TreeType::PairType;
  using TreePtr = typename TreeType::Ptr;
  TreePtr tree_;

 public:
  void add(const Key& k, const Node& v) {
    tree_ = merge(tree_, TreeType::make(k, v));
  }
  bool exists(const Key& k, const Node& v) const {
    return exists(tree_, std::make_tuple(k, v));
  }
  NodeSet lookup(const Key& k) const {
    NodeSet nodes;
    lookup(tree_, k, nodes);
    return nodes;
  }
  void merge(const Map& m) { tree_ = merge(tree_, m.tree_); }
  std::size_t size() const { return TreeType::size(tree_); }

 private:
  static TreePtr merge(const TreePtr& x, const TreePtr& y) {
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
  static void lookup(const TreePtr& x, const Key& key, NodeSet& nodes) {
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
  static bool exists(const TreePtr& x, const PairType& v) {
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
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_B24913BE_A7FB_43A0_9D42_907C04FE896D
