#include "component.hpp"
#include <llvm/IR/Instructions.h>
#include "utility.hpp"

namespace stacksafe {

Component::Component(const Blocks& b) : blocks_{b}, graph_{} {}
const Blocks& Component::blocks() const {
  return blocks_;
}
std::size_t Component::size() const {
  return graph_.size();
}
bool Component::is_safe() const {
  const auto pred = [& self = *this](BB b) { return self.check_return(b); };
  return check_global() && std::all_of(blocks_.begin(), blocks_.end(), pred);
}
void Component::add_pred(const Component& c) {
  preds_.insert(&c.graph_);
  for (const auto& pred : c.preds_) {
    preds_.insert(pred);
  }
}
void Component::init(const llvm::Function& f) {
  const NodeSet g{Node::get_global()};
  connect(g, g);
  for (const auto& a : f.args()) {
    connect(a, g);
  }
}
void Component::connect(const NodeSet& tails, const NodeSet& heads) {
  for (const auto& tail : tails) {
    for (const auto& head : heads) {
      const auto p = [&tail, &head](const Graph* g) {
        return g->contains(tail, head);
      };
      if (std::none_of(preds_.begin(), preds_.end(), p)) {
        graph_.connect(NodeSet{tail}, NodeSet{head});
      }
    }
  }
}
void Component::connect(const llvm::Value& tail, const NodeSet& heads) {
  for (const auto& head : heads) {
    const auto p = [&tail, &head](const Graph* g) {
      return g->contains(tail, head);
    };
    if (std::none_of(preds_.begin(), preds_.end(), p)) {
      graph_.connect(tail, NodeSet{head});
    }
  }
}
void Component::followings(const NodeSet& tails, NodeSet& heads) const {
  for (const auto& pred : preds_) {
    pred->followings(tails, heads);
  }
  graph_.followings(tails, heads);
}
void Component::followings(const llvm::Value& tail, NodeSet& heads) const {
  if (is_global(tail)) {
    followings(NodeSet{Node::get_global()}, heads);
  } else {
    for (const auto& pred : preds_) {
      pred->followings(tail, heads);
    }
    graph_.followings(tail, heads);
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
