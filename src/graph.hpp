#ifndef INCLUDE_GUARD_E1E5ACD3_3435_4167_A6B2_0E8D6A2A38AC
#define INCLUDE_GUARD_E1E5ACD3_3435_4167_A6B2_0E8D6A2A38AC

#include <set>
#include <vector>
#include "node.hpp"

namespace llvm {
class Function;
}

namespace stacksafe {

class NodeSet : private std::set<Node> {
  using Super = std::set<Node>;

 public:
  using Super::begin, Super::end, Super::insert;
  void merge(const NodeSet& nodes);
};

class Graph : private std::vector<Edge> {
  using Super = std::vector<Edge>;
  using iterator = Super::iterator;
  using Result = std::tuple<iterator, bool>;

 public:
  void init(const llvm::Function& f);
  bool append(const Node& tail, const Node& head);
  NodeSet heads(const Node& tail) const;
  NodeSet tails() const;
  bool merge(const Graph& g);

 private:
  Result insert(iterator hint, const Edge& e);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_E1E5ACD3_3435_4167_A6B2_0E8D6A2A38AC
