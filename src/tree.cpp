#include "tree.hpp"
#include <cassert>
#include "graph.hpp"

namespace stacksafe {

Header::Header() : rank_{0}, size_{0} {}
Header::Header(const TreePtr &x, const TreePtr &y)
    : rank_{calc_rank(x, y)}, size_{calc_size(x, y)} {}
std::size_t Header::calc_rank(const TreePtr &x, const TreePtr &y) {
  const auto kx = Tree::get_rank(x) + (Tree::is_black(x) ? 1 : 0);
  const auto ky = Tree::get_rank(y) + (Tree::is_black(y) ? 1 : 0);
  assert(kx == ky);
  return kx;
}
std::size_t Header::calc_size(const TreePtr &x, const TreePtr &y) {
  return Tree::get_size(x) + Tree::get_size(y) + 1;
}

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
    return join(merge(l, y->left_), y->value_, merge(r, y->right_));
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
bool RedBlackTree::is_valid(const Ptr &x) {
  const auto get_value = [](const Ptr &x) { return x ? &x->value_ : nullptr; };
  const auto valid_rank = [](const Ptr &x, const Ptr &y) {
    return get_rank(x) == get_rank(y) + (is_black(y) ? 1 : 0);
  };
  const auto valid_size = [](const Ptr &x) {
    return get_size(x) == get_size(x->left_) + get_size(x->right_) + 1;
  };
  if (x && is_valid(x->left_) && is_valid(x->right_)) {
    const auto lv = get_value(x->left_);
    const auto rv = get_value(x->right_);
    const auto lb = !lv || (*lv < x->value_);
    const auto rb = !rv || (x->value_ < *rv);
    const auto lk = valid_rank(x, x->left_);
    const auto rk = valid_rank(x, x->right_);
    const auto s = valid_size(x);
    const auto c = is_black(x) || (is_black(x->left_) && is_black(x->right_));
    return lb && rb && lk && rk && s && c;
  }
  return !x;
}
auto RedBlackTree::branch(const Ptr &l, const Ptr &c, const Ptr &r, bool b)
    -> Ptr {
  return branch(l, r, b, c->value_);
}
auto RedBlackTree::as_root(const Ptr &x) -> Ptr {
  return is_black(x) ? x : black(x);
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
    t = red(as_root(x), v, y);
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
  return red(x, v, as_root(y));
}
auto RedBlackTree::join(const Ptr &x, PairType v, const Ptr &y) -> Ptr {
  if (less_rank(x, y)) {
    const auto z = join_left(x, v, y);
    return is_red_twice(z) ? as_root(z) : z;
  } else if (less_rank(y, x)) {
    const auto z = join_right(x, v, y);
    return is_red_twice(z) ? as_root(z) : z;
  } else if (is_black(x) || is_black(y)) {
    return red(as_root(x), v, as_root(y));
  } else {
    return branch(x, y, true, v);
  }
}
auto RedBlackTree::split(const Ptr &x, PairType v) -> Result {
  if (!x) {
    return {nullptr, false, nullptr};
  } else if (v < x->value_) {
    const auto [l, b, r] = split(x->left_, v);
    return {l, b, join(r, x->value_, x->right_)};
  } else if (x->value_ < v) {
    const auto [l, b, r] = split(x->right_, v);
    return {join(x->left_, x->value_, l), b, r};
  } else {
    return {x->left_, true, x->right_};
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
