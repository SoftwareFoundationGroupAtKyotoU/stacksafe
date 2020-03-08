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
  using Super::begin, Super::end, Super::size;
  NodeSet();
  explicit NodeSet(const Node& n);
  void merge(const NodeSet& nodes);
  bool element(const Node& n) const;
  bool has_local() const;
};

class Graph {
  using Heap = std::unordered_map<Node, NodeSet, NodeHash>;
  using Stack = std::unordered_map<const llvm::Value*, NodeSet>;
  Heap heap_;
  Stack stack_;

 public:
  std::size_t size() const;
  void init(const llvm::Function& f);
  bool contains(const Node& tail, const Node& head) const;
  bool contains(const llvm::Value& tail, const Node& head) const;
  void connect(const NodeSet& tails, const NodeSet& heads);
  void connect(const llvm::Value& tail, const NodeSet& heads);
  void followings(const NodeSet& tails, NodeSet& heads) const;
  void followings(const llvm::Value& tail, NodeSet& heads) const;
  void merge(const Graph& g);
  NodeSet reachables(const NodeSet& nodes) const;
  NodeSet reachables(const llvm::Value& v) const;

 private:
  NodeSet& heap_at(const Node& tail);
  NodeSet& stack_at(const llvm::Value& tail);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_E1E5ACD3_3435_4167_A6B2_0E8D6A2A38AC
