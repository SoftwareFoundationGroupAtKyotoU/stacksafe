#include "node.hpp"
#include <llvm/ADT/Hashing.h>

namespace stacksafe {
namespace {
constexpr std::uintptr_t mask{1};
}  // namespace

Node::Node(const llvm::AllocaInst *p) : ptr_{p} {
  assert(is_local());
}
Node::Node(std::uintptr_t v) : val_{v} {
  assert(is_global());
}
std::uintptr_t Node::embed(std::uintptr_t v) {
  return (v << 1) | mask;
}
std::uintptr_t Node::value() const {
  return val_ >> 1;
}
Node Node::get_global() {
  return Node{embed(0)};
}
Node Node::get_local(const llvm::AllocaInst &l) {
  return Node{&l};
}
bool Node::is_global() const {
  return val_ & mask;
}
bool Node::is_local() const {
  return !is_global();
}
bool Node::equals(const Node &that) const {
  return val_ == that.val_;
}
bool Node::less(const Node &that) const {
  return val_ < that.val_;
}
std::size_t Node::hash() const {
  return llvm::hash_value(val_);
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
