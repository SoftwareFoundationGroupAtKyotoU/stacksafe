#ifndef INCLUDE_GUARD_E1E5ACD3_3435_4167_A6B2_0E8D6A2A38AC
#define INCLUDE_GUARD_E1E5ACD3_3435_4167_A6B2_0E8D6A2A38AC

#include <unordered_map>
#include <unordered_set>
#include "node.hpp"

namespace llvm {
class Function;
class Value;
}  // namespace llvm

namespace stacksafe {

class NodeSet : private std::unordered_set<Node, NodeHash> {
  using Super = std::unordered_set<Node, NodeHash>;

 public:
  using Super::begin, Super::end, Super::size, Super::insert;
  NodeSet();
  explicit NodeSet(const Node& n);
  void merge(const NodeSet& nodes);
  bool element(const Node& n) const;
  bool includes(const NodeSet& that) const;
  bool has_local() const;
};

class Graph {
  using Heap = std::unordered_multimap<Node, Node, NodeHash>;
  using Stack = std::unordered_multimap<const llvm::Value*, Node>;
  Heap heap_;
  Stack stack_;

 public:
  std::size_t size() const;
  bool includes(const Graph& that) const;
  bool contains(const Node& tail, const Node& head) const;
  bool contains(const llvm::Value& tail, const Node& head) const;
  void connect(const Node& tail, const Node& head);
  void connect(const llvm::Value& tail, const Node& head);
  void followings(const NodeSet& tails, NodeSet& heads) const;
  void followings(const llvm::Value& tail, NodeSet& heads) const;
  NodeSet reachables(const NodeSet& nodes) const;
  NodeSet reachables(const llvm::Value& value) const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_E1E5ACD3_3435_4167_A6B2_0E8D6A2A38AC
