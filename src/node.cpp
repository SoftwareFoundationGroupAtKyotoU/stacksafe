#include "node.hpp"
#include <llvm/IR/Instructions.h>
#include <llvm/Support/ErrorHandling.h>
#include "utility.hpp"

namespace stacksafe {

Node::Node(Kind k, const void *p) : kind_{k}, ptr_{p} {}
Node Node::get_constant() {
  return Node{Kind::Constant, nullptr};
}
Node Node::get_global() {
  return Node{Kind::Global, nullptr};
}
Node Node::get_local(const llvm::AllocaInst &l) {
  return Node{Kind::Local, &l};
}
Node Node::get_register(const llvm::Argument &a) {
  return Node{Kind::Argument, &a};
}
Node Node::get_register(const llvm::Instruction &i) {
  return Node{Kind::Register, &i};
}
Node Node::from_value(const llvm::Value &v) {
  if (auto c = llvm::dyn_cast<llvm::Constant>(&v)) {
    return is_global(*c) ? Node::get_global() : Node::get_constant();
  } else if (auto i = llvm::dyn_cast<llvm::Instruction>(&v)) {
    assert(is_register(*i) && "invalid register lookup");
    return Node::get_register(*i);
  } else if (auto a = llvm::dyn_cast<llvm::Argument>(&v)) {
    return Node::get_register(*a);
  } else {
    llvm_unreachable("invalid value lookup");
  };
}
std::pair<std::size_t, const void *> Node::pair() const {
  return {static_cast<std::size_t>(kind_), ptr_};
}
bool Node::is_reg() const {
  return kind_ == Kind::Register || kind_ == Kind::Argument;
}
bool Node::is_symbol() const {
  return !is_reg();
}
bool Node::is_local() const {
  return kind_ == Kind::Local;
}
bool Node::equals(const Node &that) const {
  return kind_ == that.kind_ && ptr_ == that.ptr_;
}
bool Node::less(const Node &that) const {
  if (kind_ == that.kind_) {
    return ptr_ < that.ptr_;
  }
  return kind_ < that.kind_;
}
bool operator==(const Node &lhs, const Node &rhs) {
  return lhs.equals(rhs);
}
bool operator<(const Node &lhs, const Node &rhs) {
  return lhs.less(rhs);
}

void NodeSet::merge(const NodeSet &nodes) {
  Super::insert(nodes.begin(), nodes.end());
}
bool NodeSet::element(const Node &n) const {
  return 0 != Super::count(n);
}
bool NodeSet::has_local() const {
  for (const auto &n : *this) {
    if (n.is_local()) {
      return true;
    }
  }
  return false;
}

}  // namespace stacksafe
