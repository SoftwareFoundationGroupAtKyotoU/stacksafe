#include "pointsto.hpp"
#include <llvm/Support/Debug.h>
#include "graph.hpp"
#include "params.hpp"
#include "utility.hpp"

namespace stacksafe {

PointsTo::PointsTo(Graph &g) : graph_{g}, updated_{false} {}
void PointsTo::binop(const llvm::Instruction &dst, const llvm::Value &lhs,
                     const llvm::Value &rhs) {
  NodeSet heads;
  heads.merge(lookup(lhs));
  heads.merge(lookup(rhs));
  append(dst, heads);
}
void PointsTo::alloc(const llvm::AllocaInst &dst) {
  NodeSet heads;
  heads.insert(Node{Symbol{dst}});
  append(dst, heads);
}
void PointsTo::load(const llvm::Instruction &dst, const llvm::Value &src) {
  NodeSet heads;
  for (const auto &sym : lookup(src)) {
    heads.merge(graph_.heads(sym));
  }
  append(dst, heads);
}
void PointsTo::store(const llvm::Value &src, const llvm::Value &dst) {
  const auto heads = lookup(src);
  for (const auto &tail : lookup(dst)) {
    for (const auto &head : heads) {
      update(graph_.append(tail, head));
    }
  }
}
void PointsTo::cmpxchg(const llvm::Instruction &dst, const llvm::Value &ptr,
                       const llvm::Value &val) {
  load(dst, ptr);
  store(val, ptr);
}
void PointsTo::cast(const llvm::Instruction &dst, const llvm::Value &src) {
  append(dst, lookup(src));
}
void PointsTo::phi(const llvm::Instruction &dst, const Params &params) {
  NodeSet heads;
  for (const auto &arg : params) {
    heads.merge(lookup(arg));
  }
  append(dst, heads);
}
void PointsTo::constant(const llvm::Instruction &) {}
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
