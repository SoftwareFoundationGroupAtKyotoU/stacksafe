#include "component.hpp"
#include <llvm/IR/Instructions.h>

namespace stacksafe {

Component::Component(const Blocks& b) : blocks_{b}, graph_{} {}
const Blocks& Component::blocks() const {
  return blocks_;
}
Graph& Component::graph() {
  return graph_;
}
bool Component::is_safe() const {
  const auto pred = [& self = *this](BB b) { return self.check_return(b); };
  return check_global() && std::all_of(blocks_.begin(), blocks_.end(), pred);
}
void Component::add_pred(const Graph& g) {
  preds_.push_back(&g);
}
void Component::transfer() {
  for (auto&& pred : preds_) {
    graph_.merge(*pred);
  }
}
bool Component::check_global() const {
  return !graph_.reachables(NodeSet{Node::get_global()}).has_local();
}
bool Component::check_return(BB b) const {
  const auto t = b->getTerminator();
  if (const auto i = llvm::dyn_cast<llvm::ReturnInst>(t)) {
    if (const auto v = i->getReturnValue()) {
      return !graph_.reachables(*v).has_local();
    }
  }
  return true;
}

}  // namespace stacksafe
