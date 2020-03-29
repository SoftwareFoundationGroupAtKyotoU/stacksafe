#include "node.hpp"
#include <cassert>
#include <functional>

namespace stacksafe {
namespace {
constexpr std::uintptr_t mask{1};
}  // namespace

Node::Node(const llvm::AllocaInst &l) : local_{&l} {
  assert(is_local());
}
Node::Node(const llvm::Instruction &r) : reg_{&r} {
  assert(is_local());
}
Node::Node(std::uintptr_t v) : val_{v} {
  assert(!is_local());
}
std::uintptr_t Node::embed(std::uintptr_t v) {
  return (v << 1) | mask;
}
std::uintptr_t Node::value() const {
  return val_ >> 1;
}
Node Node::get_global() {
  return Node{embed(-1)};
}
Node Node::get_local(const llvm::AllocaInst &l) {
  return Node{l};
}
Node Node::get_register(const llvm::Instruction &r) {
  return Node{r};
}
bool Node::is_local() const {
  return (val_ & mask) == 0;
}
bool Node::equals(const Node &that) const {
  return val_ == that.val_;
}
bool Node::less(const Node &that) const {
  return val_ < that.val_;
}
std::size_t Node::hash() const {
  return std::hash<std::uintptr_t>{}(val_);
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

NodeSet::NodeSet() = default;
NodeSet::NodeSet(const Node &n) {
  Super::insert(n);
}
void NodeSet::merge(const NodeSet &nodes) {
  Super::insert(nodes.begin(), nodes.end());
}
bool NodeSet::element(const Node &n) const {
  return 0 != Super::count(n);
}
bool NodeSet::includes(const NodeSet &that) const {
  for (const auto &n : that) {
    if (element(n)) {
      continue;
    }
    return false;
  }
  return true;
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
