#ifndef INCLUDE_GUARD_B24913BE_A7FB_43A0_9D42_907C04FE896D
#define INCLUDE_GUARD_B24913BE_A7FB_43A0_9D42_907C04FE896D

#include "tree.hpp"

namespace stacksafe {
class NodeSet;

class Map {
  using Tree = rbtree::Tree;
  using PairType = rbtree::PairType;
  TreePtr tree_;

 public:
  void add(const Node& k, const Node& v);
  bool exists(const Node& k, const Node& v) const;
  NodeSet lookup(const Node& k) const;
  void merge(const Map& m);
  std::size_t size() const;

 private:
  static TreePtr merge(const TreePtr& x, const TreePtr& y);
  static void lookup(const TreePtr& x, const Node& key, NodeSet& nodes);
  static bool exists(const TreePtr& x, const PairType& v);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_B24913BE_A7FB_43A0_9D42_907C04FE896D
