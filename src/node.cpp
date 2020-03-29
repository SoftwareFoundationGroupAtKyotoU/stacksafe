#include "node.hpp"
#include <algorithm>
#include <cassert>
#include <functional>
#include <limits>

namespace stacksafe {
namespace {
constexpr std::uintptr_t mask{1};
constexpr std::uintptr_t global{std::numeric_limits<std::uintptr_t>::max()};
std::uintptr_t embed(std::uintptr_t v) {
  return (v << 1) | mask;
}
std::uintptr_t extract(std::uintptr_t v) {
  return v >> 1;
}
bool flag(std::uintptr_t v) {
  return static_cast<bool>(v & mask);
}
}  // namespace

Node::Node(const llvm::AllocaInst &l) : local_{&l} {
  assert(!is_global());
}
Node::Node(const llvm::Value &r) : reg_{&r} {
  assert(!is_global());
}
Node::Node(std::uintptr_t v) : val_{embed(v)} {
  assert(is_global());
}
Node Node::get_global() {
  return Node{global};
}
Node Node::get_argument(std::uintptr_t a) {
  return Node{a};
}
Node Node::get_local(const llvm::AllocaInst &l) {
  return Node{l};
}
Node Node::get_register(const llvm::Value &r) {
  return Node{r};
}
std::uintptr_t Node::value() const {
  return extract(val_);
}
const void *Node::ptr() const {
  return ptr_;
}
bool Node::is_global() const {
  return flag(val_);
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
  const auto has = [&self = *this](const Node &n) { return self.element(n); };
  return std::all_of(that.begin(), that.end(), has);
}
bool NodeSet::has_local() const {
  const auto is_local = [](const Node &n) { return !n.is_global(); };
  return std::any_of(begin(), end(), is_local);
}

}  // namespace stacksafe
