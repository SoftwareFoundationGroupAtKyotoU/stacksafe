#ifndef INCLUDE_GUARD_E1E5ACD3_3435_4167_A6B2_0E8D6A2A38AC
#define INCLUDE_GUARD_E1E5ACD3_3435_4167_A6B2_0E8D6A2A38AC

#include <tuple>
#include <vector>
#include "node.hpp"

namespace llvm {
class Function;
class Value;
}  // namespace llvm

namespace stacksafe {

class Edge : private std::tuple<Node, Node> {
  using Super = std::tuple<Node, Node>;

 public:
  using Super::Super;
  explicit Edge(const Node& n);
  const Node& tail() const;
  const Node& head() const;
  const Super& pair() const;
};
bool operator==(const Edge& lhs, const Edge& rhs);
bool operator<(const Edge& lhs, const Edge& rhs);

class Graph : private std::vector<Edge> {
  using Super = std::vector<Edge>;
  using iterator = Super::iterator;
  using Result = std::tuple<iterator, bool>;

 public:
  using Super::size;
  void init(const llvm::Function& f);
  void connect(const Node& tail, const Node& head);
  NodeSet followings(const Node& tail) const;
  void merge(const Graph& g);
  void reachables(const Node& n, NodeSet& nodes) const;
  void reachables(const llvm::Value& v, NodeSet& nodes) const;

 private:
  Result insert(iterator hint, const Edge& e);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_E1E5ACD3_3435_4167_A6B2_0E8D6A2A38AC
