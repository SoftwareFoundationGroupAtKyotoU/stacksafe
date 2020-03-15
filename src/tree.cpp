#include "tree.hpp"
#include <cassert>

namespace tree {

Node::Node(const Ptr &l, const Ptr &r, bool b, int k, int s, int v)
    : left_{l}, right_{r}, black_{b}, rank_{k}, size_{s}, value_{v} {}
auto Node::leaf(int v) -> Ptr {
  return std::make_shared<Helper>(nullptr, nullptr, true, 0, 1, v);
}
auto Node::branch(const Ptr &l, const Ptr &r, bool b) -> Ptr {
  return std::make_shared<Helper>(l, r, b, calc_rank(l, r), calc_size(l, r), 0);
}
bool Node::is_black(const Ptr &x) {
  return x->black_;
}
bool Node::is_triple(const Ptr &x) {
  return !is_black(x) && !is_black(x->left_);
}
auto Node::red(const Ptr &l, const Ptr &r) -> Ptr {
  return branch(l, r, false);
}
auto Node::black(const Ptr &l, const Ptr &r) -> Ptr {
  return branch(l, r, true);
}
auto Node::black(const Ptr &x) -> Ptr {
  return black(x->left_, x->right_);
}
auto Node::merge_left(const Ptr &x, const Ptr &y) -> Ptr {
  assert(x->rank_ <= y->rank_);
  assert(is_black(x));
  Ptr t = nullptr;
  if (less_rank(x, y)) {
    const auto l = y->left_;
    const auto r = y->right_;
    const auto z = merge_left(x, l);
    if (is_triple(z)) {
      assert(is_black(y) && !is_black(l));
      if (is_black(r)) {
        t = black(z->left_, red(z->right_, r));
      } else {
        t = red(black(z), black(r));
      }
    } else {
      if (is_black(y)) {
        t = black(z, r);
      } else {
        t = red(z, r);
        assert(is_black(z) || is_triple(t));
      }
    }
  } else {
    assert(is_black(y));
    t = red(x, y);
  }
  return t;
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
