#include "tree.hpp"
#include <cassert>
#include "graph.hpp"

namespace stacksafe {

RedBlackTree::RedBlackTree(const Ptr &l, const Ptr &r, bool b, int k, int s,
                           PairType v)
    : left_{l}, right_{r}, black_{b}, rank_{k}, size_{s}, value_{v} {}
auto RedBlackTree::leaf() -> Ptr {
  return nullptr;
}
auto RedBlackTree::single(PairType v) -> Ptr {
  return branch(nullptr, nullptr, true, v);
}
auto RedBlackTree::branch(const Ptr &l, const Ptr &r, bool b, PairType v)
    -> Ptr {
  const auto calc_rank = [](const Ptr &x) {
    return get_rank(x) + (is_black(x) ? 1 : 0);
  };
  const auto k = calc_rank(l);
  const auto s = get_size(l) + get_size(r) + 1;
  assert(k == calc_rank(r));
  return std::make_shared<Helper>(l, r, b, k, s, v);
}
bool RedBlackTree::exists(const Ptr &x, PairType v) {
  if (!x) {
    return false;
  } else if (x->value_ < v) {
    return exists(x->right_, v);
  } else if (v < x->value_) {
    return exists(x->left_, v);
  } else {
    return true;
  }
}
auto RedBlackTree::merge(const Ptr &x, const Ptr &y) -> Ptr {
  if (get_size(x) < get_size(y)) {
    return merge(y, x);
  } else if (x && y) {
    const auto [l, b, r] = split(x, y->value_);
    return join(merge(l, as_root(y->left_)), y->value_,
                merge(r, as_root(y->right_)));
  } else {
    return x ? x : y;
  }
}
NodeSet RedBlackTree::find(const Ptr &x, const Node &key) {
  NodeSet values;
  find(x, key, values);
  return values;
}
void RedBlackTree::find(const Ptr &x, const Node &key, NodeSet &values) {
  if (x) {
    const auto [k, v] = x->value_;
    if (k < key) {
      find(x->right_, key, values);
    } else if (key < k) {
      find(x->left_, key, values);
    } else {
      find(x->left_, key, values);
      values.merge(NodeSet{v});
      find(x->right_, key, values);
    }
  }
}
bool RedBlackTree::is_black(const Ptr &x) {
  return !x || x->black_;
}
bool RedBlackTree::is_red_twice(const Ptr &x) {
  return !is_black(x) && !(is_black(x->left_) && is_black(x->right_));
}
auto RedBlackTree::branch(const Ptr &l, const Ptr &c, const Ptr &r, bool b)
    -> Ptr {
  return branch(l, r, b, c->value_);
}
auto RedBlackTree::as_root(const Ptr &x) -> Ptr {
  return (x && !is_black(x)) ? black(x) : x;
}
auto RedBlackTree::red(const Ptr &l, const Ptr &c, const Ptr &r) -> Ptr {
  return branch(l, c, r, false);
}
auto RedBlackTree::red(const Ptr &l, PairType v, const Ptr &r) -> Ptr {
  return branch(l, r, false, v);
}
auto RedBlackTree::black(const Ptr &l, const Ptr &c, const Ptr &r) -> Ptr {
  return branch(l, c, r, true);
}
auto RedBlackTree::black(const Ptr &x) -> Ptr {
  assert(!is_black(x));
  return black(x->left_, x, x->right_);
}
auto RedBlackTree::join_left(const Ptr &x, PairType v, const Ptr &y) -> Ptr {
  assert(get_rank(x) <= get_rank(y));
  assert(is_black(x));
  Ptr t = nullptr;
  if (less_rank(x, y)) {
    assert(y);
    const auto l = y->left_;
    const auto r = y->right_;
    const auto z = join_left(x, v, l);
    if (is_red_twice(z)) {
      assert(is_black(y) && !is_black(l));
      if (is_black(r)) {
        t = black(z->left_, z, red(z->right_, y, r));
      } else {
        t = red(black(z), y, black(r));
      }
    } else {
      if (is_black(y)) {
        t = black(z, y, r);
      } else {
        t = red(z, y, r);
        assert(is_black(z) || is_red_twice(t));
      }
    }
  } else {
    assert(is_black(y));
    t = red(x, v, y);
  }
  return t;
}
auto RedBlackTree::join_right(const Ptr &x, PairType v, const Ptr &y) -> Ptr {
  if (less_rank(y, x)) {
    const auto l = x->left_;
    const auto z = join_right(x->right_, v, y);
    if (is_red_twice(z)) {
      return is_black(l) ? black(red(l, x, z->left_), z, z->right_) :
                           red(black(l), x, black(z));
    }
    return is_black(x) ? black(l, x, z) : red(l, x, z);
  }
  return red(x, v, y);
}
auto RedBlackTree::join(const Ptr &x, PairType v, const Ptr &y) -> Ptr {
  const auto z = less_rank(x, y) ? join_left(x, v, y) : join_right(x, v, y);
  return is_black(z) ? z : black(z);
}
auto RedBlackTree::split(const Ptr &x, PairType v) -> Result {
  const auto as_root = [](const Ptr &p) {
    return (p && !is_black(p)) ? black(p) : p;
  };
  if (!x) {
    return {nullptr, false, nullptr};
  } else if (v < x->value_) {
    const auto [l, b, r] = split(x->left_, v);
    return {l, b, join(r, x->value_, as_root(x->right_))};
  } else if (x->value_ < v) {
    const auto [l, b, r] = split(x->right_, v);
    return {join(as_root(x->left_), x->value_, l), b, r};
  } else {
    return {as_root(x->left_), true, as_root(x->right_)};
  }
}
int RedBlackTree::get_rank(const Ptr &x) {
  return x ? x->rank_ : 0;
}
int RedBlackTree::get_size(const Ptr &x) {
  return x ? x->size_ : 0;
}
bool RedBlackTree::less_rank(const Ptr &x, const Ptr &y) {
  return get_rank(x) < get_rank(y);
}

}  // namespace stacksafe
