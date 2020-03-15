#include "tree.hpp"
#include <cassert>

namespace tree {

Node::Node(Ptr l, Ptr r, bool b, int k, int s, int v)
    : left_{l}, right_{r}, black_{b}, rank_{k}, size_{s}, value_{v} {}
auto Node::leaf(int v) -> Ptr {
  return std::make_shared<Helper>(nullptr, nullptr, true, 0, 1, v);
}
auto Node::branch(Ptr l, Ptr r, bool b) -> Ptr {
  return std::make_shared<Helper>(l, r, b, calc_rank(l, r), calc_size(l, r), 0);
}
bool Node::is_black(const Ptr &x) {
  return x->black_;
}
bool Node::is_triple(const Ptr &x) {
  return !is_black(x) && !is_black(x->left_);
}
auto Node::red(Ptr l, Ptr r) -> Ptr {
  return branch(l, r, false);
}
auto Node::black(Ptr l, Ptr r) -> Ptr {
  return branch(l, r, true);
}
auto Node::black(Ptr x) -> Ptr {
  return black(x->left_, x->right_);
}
int Node::calc_rank(const Ptr &x, const Ptr &y) {
  assert(x && y);
  const auto k = x->rank_ + (x->black_ ? 1 : 0);
  assert(k == (y->rank_ + (y->black_ ? 1 : 0)));
  return k;
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
