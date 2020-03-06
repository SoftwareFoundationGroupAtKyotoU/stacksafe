#include "node.hpp"
#include <llvm/ADT/Hashing.h>

namespace stacksafe {
namespace {
constexpr std::uintptr_t mask{1};
std::uintptr_t embed(std::uintptr_t v) {
  return (v << 1) | mask;
}
}  // namespace

Node::Node(Kind k, const llvm::AllocaInst *p) : kind_{k}, ptr_{p} {
  assert(is_local());
}
Node::Node(std::uintptr_t v) : kind_{Kind::Global}, val_{v} {
  assert(is_global());
}
Node Node::get_global() {
  return Node{embed(0)};
}
Node Node::get_local(const llvm::AllocaInst &l) {
  return Node{Kind::Local, &l};
}
bool Node::is_global() const {
  return val_ & mask;
}
bool Node::is_local() const {
  return !is_global();
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
