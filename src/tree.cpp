#include "tree.hpp"
#include <cassert>
#include "graph.hpp"

namespace stacksafe {

Header::Header(const TreePtr &x, const TreePtr &y, bool b)
    : rank_{calc_rank(x, y) + (b ? 1 : 0)},
      size_{calc_size(x, y)},
      left_{x},
      right_{y} {}
std::size_t Header::rank() const {
  return rank_ >> 1;
}
std::size_t Header::size() const {
  return size_;
}
bool Header::is_black() const {
  return rank_ & 1;
}
const TreePtr Header::left() const {
  return left_;
}
const TreePtr Header::right() const {
  return right_;
}
std::size_t Header::calc_rank(const TreePtr &x, const TreePtr &y) {
  const auto kx = Tree::get_rank(x) + (Tree::is_black(x) ? 1 : 0);
  const auto ky = Tree::get_rank(y) + (Tree::is_black(y) ? 1 : 0);
  assert(kx == ky);
  return kx << 1;
}
std::size_t Header::calc_size(const TreePtr &x, const TreePtr &y) {
  return Tree::get_size(x) + Tree::get_size(y) + 1;
}

RedBlackTree::RedBlackTree(const Ptr &l, const Ptr &r, bool b, PairType v)
    : Header{l, r, b}, value_{v} {}
auto RedBlackTree::value(const Ptr &x) -> const PairType & {
  return x->value_;
}
auto RedBlackTree::leaf() -> Ptr {
  return nullptr;
}
auto RedBlackTree::single(PairType v) -> Ptr {
  return branch(nullptr, nullptr, true, v);
}
auto RedBlackTree::branch(const Ptr &l, const Ptr &r, bool b, PairType v)
    -> Ptr {
  return std::make_shared<Helper>(l, r, b, v);
}
bool RedBlackTree::exists(const Ptr &x, PairType v) {
  if (!x) {
    return false;
  } else if (value(x) < v) {
    return exists(x->right(), v);
  } else if (v < value(x)) {
    return exists(x->left(), v);
  } else {
    return true;
  }
}
auto RedBlackTree::merge(const Ptr &x, const Ptr &y) -> Ptr {
  if (get_size(x) < get_size(y)) {
    return merge(y, x);
  } else if (x && y) {
    const auto [l, b, r] = split(x, value(y));
    return join(merge(l, y->left()), value(y), merge(r, y->right()));
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
    const auto [k, v] = value(x);
    if (k < key) {
      find(x->right(), key, values);
    } else if (key < k) {
      find(x->left(), key, values);
    } else {
      find(x->left(), key, values);
      values.merge(NodeSet{v});
      find(x->right(), key, values);
    }
  }
}
bool RedBlackTree::is_black(const Ptr &x) {
  return x ? x->is_black() : true;
}
bool RedBlackTree::is_red_twice(const Ptr &x) {
  return !is_black(x) && !(is_black(x->left()) && is_black(x->right()));
}
bool RedBlackTree::is_valid(const Ptr &x) {
  const auto get_value = [](const Ptr &x) { return x ? &value(x) : nullptr; };
  const auto valid_rank = [](const Ptr &x, const Ptr &y) {
    return get_rank(x) == get_rank(y) + (is_black(y) ? 1 : 0);
  };
  const auto valid_size = [](const Ptr &x) {
    return get_size(x) == get_size(x->left()) + get_size(x->right()) + 1;
  };
  if (x && is_valid(x->left()) && is_valid(x->right())) {
    const auto lv = get_value(x->left());
    const auto rv = get_value(x->right());
    const auto lb = !lv || (*lv < value(x));
    const auto rb = !rv || (value(x) < *rv);
    const auto lk = valid_rank(x, x->left());
    const auto rk = valid_rank(x, x->right());
    const auto s = valid_size(x);
    const auto c = is_black(x) || (is_black(x->left()) && is_black(x->right()));
    return lb && rb && lk && rk && s && c;
  }
  return !x;
}
auto RedBlackTree::branch(const Ptr &l, const Ptr &c, const Ptr &r, bool b)
    -> Ptr {
  return branch(l, r, b, value(c));
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
  return black(x->left(), x, x->right());
}
auto RedBlackTree::join_left(const Ptr &x, PairType v, const Ptr &y) -> Ptr {
  assert(get_rank(x) <= get_rank(y));
  Ptr t = nullptr;
  if (less_rank(x, y)) {
    assert(y);
    const auto l = y->left();
    const auto r = y->right();
    const auto z = join_left(x, v, l);
    if (is_red_twice(z)) {
      assert(is_black(y) && !is_black(l));
      if (is_black(r)) {
        t = black(z->left(), z, red(z->right(), y, r));
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
    const auto l = x->left();
    const auto z = join_right(x->right(), v, y);
    if (is_red_twice(z)) {
      return is_black(l) ? black(red(l, x, z->left()), z, z->right()) :
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
  } else if (v < value(x)) {
    const auto [l, b, r] = split(x->left(), v);
    return {l, b, join(r, value(x), x->right())};
  } else if (value(x) < v) {
    const auto [l, b, r] = split(x->right(), v);
    return {join(x->left(), value(x), l), b, r};
  } else {
    return {x->left(), true, x->right()};
  }
}
int RedBlackTree::get_rank(const Ptr &x) {
  return x ? x->rank() : 0;
}
int RedBlackTree::get_size(const Ptr &x) {
  return x ? x->size() : 0;
}
bool RedBlackTree::less_rank(const Ptr &x, const Ptr &y) {
  return get_rank(x) < get_rank(y);
}

}  // namespace stacksafe
