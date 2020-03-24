#include "graph.hpp"
#include <llvm/IR/Function.h>
#include "utility.hpp"

namespace stacksafe {

std::size_t Graph::size() const {
  return heap_.size() + stack_.size();
}
void Graph::merge(const Graph& g) {
  merge(g.heap_);
  merge(g.stack_);
}
void Graph::merge(const Heap& heap) {
  heap_.merge(heap);
}
void Graph::merge(const Stack& stack) {
  stack_.merge(stack);
}
bool Graph::contains(const Node& tail, const Node& head) const {
  return heap_.exists(tail, head);
}
bool Graph::contains(const llvm::Value& tail, const Node& head) const {
  return stack_.exists(&tail, head);
}
void Graph::connect(const Node& tail, const Node& head) {
  heap_.add(tail, head);
}
void Graph::connect(const llvm::Value& tail, const Node& head) {
  stack_.add(&tail, head);
}
void Graph::followings(const NodeSet& tails, NodeSet& heads) const {
  for (const auto& tail : tails) {
    heads.merge(heap_.lookup(tail));
  }
}
void Graph::followings(const llvm::Value& tail, NodeSet& heads) const {
  if (is_global(tail)) {
    followings(NodeSet{Node::get_global()}, heads);
  } else {
    heads.merge(stack_.lookup(&tail));
  }
}
NodeSet Graph::reachables(const NodeSet& nodes) const {
  NodeSet tails, heads{nodes};
  while (tails.size() < heads.size()) {
    tails = heads;
    followings(tails, heads);
  }
  return heads;
}
NodeSet Graph::reachables(const llvm::Value& value) const {
  NodeSet nodes;
  followings(value, nodes);
  return reachables(nodes);
}

}  // namespace stacksafe
