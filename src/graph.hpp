#ifndef INCLUDE_GUARD_E1E5ACD3_3435_4167_A6B2_0E8D6A2A38AC
#define INCLUDE_GUARD_E1E5ACD3_3435_4167_A6B2_0E8D6A2A38AC

#include <map>
#include <tuple>
#include <unordered_map>
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

class Graph {
  using Heap = std::map<Node, NodeSet>;
  using Stack = std::unordered_map<const llvm::Value*, NodeSet>;
  Heap heap_;
  Stack stack_;

 public:
  std::size_t size() const;
  void init(const llvm::Function& f);
  void connect(const NodeSet& tails, const NodeSet& heads);
  void connect(const llvm::Value& tail, const NodeSet& heads);
  void followings(const NodeSet& tails, NodeSet& heads) const;
  void followings(const llvm::Value& tail, NodeSet& heads) const;
  void merge(const Graph& g);
  NodeSet reachables(const NodeSet& nodes) const;
  NodeSet reachables(const llvm::Value& v) const;

 private:
  NodeSet& at(const Node& tail);
  NodeSet& stack_at(const llvm::Value& tail);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_E1E5ACD3_3435_4167_A6B2_0E8D6A2A38AC
