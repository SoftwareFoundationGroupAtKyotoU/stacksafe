#include "pointsto.hpp"
#include <llvm/Support/Debug.h>
#include "graph.hpp"
#include "utility.hpp"

namespace stacksafe {

PointsTo::PointsTo(Graph &g) : graph_{g}, updated_{false} {}
NodeSet PointsTo::lookup(const Symbol &tail) const {
  return graph_.heads(Node{tail});
}
NodeSet PointsTo::lookup(const llvm::Value &tail) const {
  const auto v = &tail;
  if (auto c = llvm::dyn_cast<llvm::Constant>(v)) {
    NodeSet nodes;
    if (is_global(*c)) {
      nodes.insert(Node{Symbol::get_global()});
    }
    return nodes;
  } else if (auto i = llvm::dyn_cast<llvm::Instruction>(v)) {
    assert(is_register(*i) && "invalid register lookup");
    return graph_.heads(Node{Register{*i}});
  } else if (auto a = llvm::dyn_cast<llvm::Argument>(v)) {
    return graph_.heads(Node{Register{*a}});
  } else {
    llvm_unreachable("invalid value lookup");
  };
}
void PointsTo::append(const Symbol &tail, const NodeSet &heads) {
  for (const auto &h : heads) {
    update(graph_.append(Node{tail}, h));
  }
}
void PointsTo::append(const llvm::Instruction &tail, const NodeSet &heads) {
  for (const auto &h : heads) {
    update(graph_.append(Node{Register{tail}}, h));
  }
}
void PointsTo::update(bool updated) {
  if (updated) {
    updated_ = true;
  }
}

}  // namespace stacksafe
