#include "component.hpp"
#include <llvm/IR/Instructions.h>
#include "utility.hpp"

namespace stacksafe {

Component::Component(const Blocks& b) : blocks_{b} {}
const Blocks& Component::blocks() const {
  return blocks_;
}
std::size_t Component::size() const {
  return heap_.size() + stack_.size();
}
void Component::merge(const Component& c) {
  heap_.merge(c.heap_);
  stack_.merge(c.stack_);
}
bool Component::is_safe() const {
  const auto pred = [& self = *this](BB b) { return self.check_return(b); };
  return check_global() && std::all_of(blocks_.begin(), blocks_.end(), pred);
}
void Component::init(const llvm::Function& f) {
  const auto g = Node::get_global();
  heap_.add(g, g);
  for (const auto& a : f.args()) {
    stack_.add(Node::get_register(a), g);
  }
}
void Component::connect(const NodeSet& tails, const NodeSet& heads) {
  Graph heap;
  for (const auto& tail : tails) {
    for (const auto& head : heads) {
      heap.add(tail, head);
    }
  }
  heap_.merge(heap);
}
void Component::connect(const llvm::Value& tail, const NodeSet& heads) {
  Graph stack;
  for (const auto& head : heads) {
    stack.add(Node::get_register(tail), head);
  }
  stack_.merge(stack);
}
void Component::followings(const NodeSet& tails, NodeSet& heads) const {
  for (const auto& tail : tails) {
    heads.merge(heap_.lookup(tail));
  }
}
void Component::followings(const llvm::Value& tail, NodeSet& heads) const {
  if (is_global(tail)) {
    followings(NodeSet{Node::get_global()}, heads);
  } else {
    heads.merge(stack_.lookup(Node::get_register(tail)));
  }
}
NodeSet Component::reachables(const Node& node) const {
  NodeSet tails, heads;
  heads.merge(NodeSet{node});
  while (tails.size() < heads.size()) {
    tails.merge(heads);
    followings(tails, heads);
  }
  return heads;
}
NodeSet Component::reachables(const llvm::Value& value) const {
  NodeSet tails, heads;
  followings(value, heads);
  while (tails.size() < heads.size()) {
    tails.merge(heads);
    followings(tails, heads);
  }
  return heads;
}
bool Component::check_global() const {
  return !reachables(Node::get_global()).has_local();
}
bool Component::check_return(BB b) const {
  const auto t = b->getTerminator();
  if (const auto i = llvm::dyn_cast<llvm::ReturnInst>(t)) {
    if (const auto v = i->getReturnValue()) {
      return !reachables(*v).has_local();
    }
  }
  return true;
}

}  // namespace stacksafe
