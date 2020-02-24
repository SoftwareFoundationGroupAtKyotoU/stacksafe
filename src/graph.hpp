#ifndef INCLUDE_GUARD_E1E5ACD3_3435_4167_A6B2_0E8D6A2A38AC
#define INCLUDE_GUARD_E1E5ACD3_3435_4167_A6B2_0E8D6A2A38AC

#include <vector>
#include "node.hpp"

namespace stacksafe {

class NodeSet : private std::vector<Node> {
  using Super = std::vector<Node>;
  friend class Graph;

 public:
  using Super::begin, Super::end;
};

class Graph : private std::vector<Edge> {
  using Super = std::vector<Edge>;
  using iterator = Super::iterator;
  using Result = std::tuple<iterator, bool>;

 public:
  bool append(const Node& tail, const Node& head);
  NodeSet outedges(const Node& tail) const;
  NodeSet tails() const;
  bool merge(const Graph& g);

 private:
  Result insert(iterator hint, const Edge& e);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_E1E5ACD3_3435_4167_A6B2_0E8D6A2A38AC
