#include "tree.hpp"
#include <cassert>
#include "graph.hpp"

namespace stacksafe {
namespace rbtree {

Header::Header(const TreePtr &x, const TreePtr &y, bool b, std::size_t k,
               std::size_t s)
    : rank_{combine(b, k)}, size_{s}, left_{x}, right_{y} {}
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
std::size_t Header::combine(bool b, std::size_t k) {
  return (k << 1) & (b ? 1 : 0);
}

TreeBase::TreeBase(const Ptr &l, const Ptr &r, bool b, PairType v)
    : Header{l, r, b, calc_rank(l), calc_size(l, r)},
      left_{l},
      right_{r},
      value_{v} {}
auto TreeBase::single(PairType v) -> Ptr {
  return branch(nullptr, nullptr, true, v);
}
auto TreeBase::value() const -> const PairType & {
  return value_;
}
int TreeBase::get_size(const Ptr &x) {
  return x ? x->size() : 0;
}
auto TreeBase::join(const Ptr &x, PairType v, const Ptr &y) -> Ptr {
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
auto TreeBase::split(const Ptr &x, PairType v) -> Result {
  if (!x) {
    return {nullptr, false, nullptr};
  } else if (v < x->value()) {
    const auto [l, b, r] = split(x->left(), v);
    return {l, b, join(r, x->value(), x->right())};
  } else if (x->value() < v) {
    const auto [l, b, r] = split(x->right(), v);
    return {join(x->left(), x->value(), l), b, r};
  } else {
    return {x->left(), true, x->right()};
  }
}

void TreeBase::find(const Ptr &x, const Node &key, NodeSet &values) {
  if (x) {
    const auto [k, v] = x->value();
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
auto TreeBase::branch(const Ptr &l, const Ptr &r, bool b, PairType v) -> Ptr {
  return std::make_shared<Helper>(l, r, b, v);
}
auto TreeBase::branch(const Ptr &l, const Ptr &c, const Ptr &r, bool b) -> Ptr {
  return branch(l, r, b, c->value());
}
auto TreeBase::red(const Ptr &l, const Ptr &c, const Ptr &r) -> Ptr {
  return branch(l, c, r, false);
}
auto TreeBase::red(const Ptr &l, PairType v, const Ptr &r) -> Ptr {
  return branch(l, r, false, v);
}
auto TreeBase::black(const Ptr &l, const Ptr &c, const Ptr &r) -> Ptr {
  return branch(l, c, r, true);
}
auto TreeBase::black(const Ptr &x) -> Ptr {
  assert(!is_black(x));
  return black(x->left(), x, x->right());
}
auto TreeBase::as_root(const Ptr &x) -> Ptr {
  return is_black(x) ? x : black(x);
}
bool TreeBase::is_black(const Ptr &x) {
  return x ? x->is_black() : true;
}
bool TreeBase::is_red_twice(const Ptr &x) {
  return !is_black(x) && !(is_black(x->left()) && is_black(x->right()));
}
bool TreeBase::is_valid(const Ptr &x) {
  const auto get_value = [](const Ptr &x) { return x ? &x->value() : nullptr; };
  const auto valid_rank = [](const Ptr &x, const Ptr &y) {
    return get_rank(x) == get_rank(y) + (is_black(y) ? 1 : 0);
  };
  const auto valid_size = [](const Ptr &x) {
    return get_size(x) == get_size(x->left()) + get_size(x->right()) + 1;
  };
  if (x && is_valid(x->left()) && is_valid(x->right())) {
    const auto lv = get_value(x->left());
    const auto rv = get_value(x->right());
    const auto lb = !lv || (*lv < x->value());
    const auto rb = !rv || (x->value() < *rv);
    const auto lk = valid_rank(x, x->left());
    const auto rk = valid_rank(x, x->right());
    const auto s = valid_size(x);
    const auto c = is_black(x) || (is_black(x->left()) && is_black(x->right()));
    return lb && rb && lk && rk && s && c;
  }
  return !x;
}
auto TreeBase::join_left(const Ptr &x, PairType v, const Ptr &y) -> Ptr {
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
auto TreeBase::join_right(const Ptr &x, PairType v, const Ptr &y) -> Ptr {
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
int TreeBase::get_rank(const Ptr &x) {
  return x ? x->rank() : 0;
}
bool TreeBase::less_rank(const Ptr &x, const Ptr &y) {
  return get_rank(x) < get_rank(y);
}
std::size_t TreeBase::calc_rank(const Ptr &x) {
  return get_rank(x) + (is_black(x) ? 1 : 0);
}
std::size_t TreeBase::calc_size(const Ptr &x, const Ptr &y) {
  return get_size(x) + get_size(y) + 1;
}

}  // namespace rbtree
}  // namespace stacksafe
