#include "tree.hpp"
#include <cassert>

namespace tree {

Node::Node(Ptr l, Ptr r, bool b, int k, int s, int v)
    : left_{l}, right_{r}, black_{b}, rank_{k}, size_{s}, value_{v} {}
auto Node::leaf(int v) -> Ptr {
  return std::make_shared<Helper>(nullptr, nullptr, true, 0, 1, v);
}
auto Node::branch(Ptr l, Ptr r, bool b) -> Ptr {
  return std::make_shared<Helper>(l, r, b, calc_rank(l), calc_size(l, r), 0);
}
auto Node::black(Ptr l, Ptr r) -> Ptr {
  return branch(l, r, true);
}
auto Node::red(Ptr l, Ptr r) -> Ptr {
  return branch(l, r, false);
}
int Node::calc_rank(const Ptr &x) {
  assert(x);
  return x->rank_ + (x->black_ ? 1 : 0);
}
int Node::calc_size(const Ptr &x, const Ptr &y) {
  assert(x && y);
  return x->size_ + y->size_;
}
bool Node::less_rank(const Ptr &x, const Ptr &y) {
  assert(x && y);
  return x->rank_ < y->rank_;
}

}  // namespace tree
