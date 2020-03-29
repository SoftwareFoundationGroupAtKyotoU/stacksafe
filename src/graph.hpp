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
  void merge(const Graph& g);

 private:
  static TreePtr merge(const TreePtr& x, const TreePtr& y);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_E1E5ACD3_3435_4167_A6B2_0E8D6A2A38AC
