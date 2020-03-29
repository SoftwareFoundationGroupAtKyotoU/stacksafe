#ifndef INCLUDE_GUARD_E1E5ACD3_3435_4167_A6B2_0E8D6A2A38AC
#define INCLUDE_GUARD_E1E5ACD3_3435_4167_A6B2_0E8D6A2A38AC

#include "node.hpp"
#include "tree.hpp"

namespace stacksafe {

class Graph {
  using TreeType = Tree<Node, Node>;
  using PairType = typename TreeType::PairType;
  using TreePtr = typename TreeType::Ptr;
  TreePtr tree_;

 public:
  void add(const Node& key, const Node& val);
  void merge(const Graph& g);
  NodeSet lookup(const Node& n) const;
  bool exists(const Node& key, const Node& val) const;
  std::size_t size() const;

 private:
  static TreePtr merge(const TreePtr& x, const TreePtr& y);
  static void lookup(const TreePtr& x, const Node& key, NodeSet& nodes);
  static bool exists(const TreePtr& x, const PairType& v);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_E1E5ACD3_3435_4167_A6B2_0E8D6A2A38AC
