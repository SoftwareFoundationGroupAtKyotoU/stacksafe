#include "node.hpp"
#include <llvm/ADT/Hashing.h>
#include <llvm/IR/Instructions.h>

namespace stacksafe {

Node::Node(Kind k, const llvm::Value *p) : kind_{k}, ptr_{p} {}
Node Node::get_global() {
  return Node{Kind::Global, nullptr};
}
Node Node::get_local(const llvm::AllocaInst &l) {
  return Node{Kind::Local, &l};
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
std::size_t Node::hash() const {
  return llvm::hash_combine(static_cast<std::size_t>(kind_), ptr_);
}
bool operator==(const Node &lhs, const Node &rhs) {
  return lhs.equals(rhs);
}
bool operator<(const Node &lhs, const Node &rhs) {
  return lhs.less(rhs);
}

std::size_t NodeHash::operator()(const Node &n) const {
  return n.hash();
}

}  // namespace stacksafe
