#include "tree.hpp"
#include <cassert>
#include "graph.hpp"

namespace stacksafe {
namespace rbtree {

Header::Header(Color c, std::size_t k, std::size_t s)
    : rank_{combine(c, k)}, size_{s} {}
std::size_t Header::rank() const {
  return rank_ >> 1;
}
std::size_t Header::size() const {
  return size_;
}
bool Header::is_black() const {
  return rank_ & 1;
}
std::size_t Header::combine(Color c, std::size_t k) {
  return (k << 1) & static_cast<std::size_t>(c);
}

TreeBase::TreeBase(const Ptr &l, const Ptr &r, Color c, const PairType &v)
    : Header{c, calc_rank(l), calc_size(l, r)},
      left_{l},
      right_{r},
      value_{v} {}
auto TreeBase::make(const Node &k, const Node &v) -> Ptr {
  return branch(Color::Red, nullptr, std::make_tuple(k, v), nullptr);
}
auto TreeBase::left() const -> const Ptr {
  return left_;
}
auto TreeBase::right() const -> const Ptr {
  return right_;
}
auto TreeBase::color() const -> Color {
  return Header::is_black() ? Color::Black : Color::Red;
}
auto TreeBase::value() const -> const PairType & {
  return value_;
}
int TreeBase::size(const Ptr &x) {
  return x ? x->Header::size() : 0;
}
int TreeBase::rank(const Ptr &x) {
  return x ? x->Header::rank() : 0;
}
auto TreeBase::join(const Ptr &x, const PairType &v, const Ptr &y) -> Ptr {
  if (rank(x) == rank(y)) {
    return (is_black(x) || is_black(y)) ?
               branch(Color::Red, as_black(x), v, as_black(y)) :
               branch(Color::Black, x, v, y);
  } else {
    const auto z =
        (rank(x) < rank(y)) ? join_left(x, v, y) : join_right(x, v, y);
    return is_red_twice(z) ? as_black(z) : z;
  }
}
auto TreeBase::split(const Ptr &x, const PairType &v) -> Result {
  Ptr l = nullptr, r = nullptr;
  bool b = false;
  if (x) {
    const auto xv = x->value();
    const auto xl = x->left();
    const auto xr = x->right();
    if (v < xv) {
      std::tie(l, b, r) = split(xl, v);
      r = join(r, xv, xr);
    } else if (xv < v) {
      std::tie(l, b, r) = split(xr, v);
      l = join(xl, xv, l);
    } else {
      return {xl, true, xr};
    }
  }
  return {l, b, r};
}

auto TreeBase::branch(Color c, const Ptr &l, const PairType &v, const Ptr &r)
    -> Ptr {
  return std::make_shared<Helper>(l, r, c, v);
}
auto TreeBase::as_black(const Ptr &x) -> Ptr {
  return is_black(x) ? x :
                       branch(Color::Black, x->left(), x->value(), x->right());
}
auto TreeBase::branch(const Ptr &l, const Ptr &r, bool b, PairType v) -> Ptr {
  return std::make_shared<Helper>(l, r, b ? Color::Black : Color::Red, v);
}
auto TreeBase::branch(const Ptr &l, const Ptr &c, const Ptr &r, bool b) -> Ptr {
  return branch(l, r, b, c->value());
}
bool TreeBase::is_black(const Ptr &x) {
  return x ? x->Header::is_black() : true;
}
bool TreeBase::is_red_twice(const Ptr &x) {
  return !is_black(x) && !(is_black(x->left()) && is_black(x->right()));
}
bool TreeBase::is_valid(const Ptr &x) {
  const auto get_value = [](const Ptr &x) { return x ? &x->value() : nullptr; };
  const auto valid_rank = [](const Ptr &x, const Ptr &y) {
    return rank(x) == rank(y) + (is_black(y) ? 1 : 0);
  };
  const auto valid_size = [](const Ptr &x) {
    return size(x) == size(x->left()) + size(x->right()) + 1;
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
auto TreeBase::join_left(const Ptr &x, const PairType &v, const Ptr &y) -> Ptr {
  assert(rank(x) <= rank(y));
  Ptr t = nullptr;
  if (rank(x) < rank(y)) {
    assert(y);
    const auto w = y->value();
    const auto l = y->left();
    const auto r = y->right();
    const auto z = join_left(x, v, l);
    if (is_red_twice(z)) {
      assert(is_black(y) && !is_black(l));
      if (is_black(r)) {
        t = branch(Color::Red, z->right(), w, r);
        t = branch(Color::Black, z->left(), z->value(), t);
      } else {
        t = branch(Color::Red, as_black(z), w, as_black(r));
      }
    } else {
      t = branch(y->color(), z, w, r);
      assert(is_black(y) || is_black(z) || is_red_twice(t));
    }
  } else {
    assert(is_black(y));
    t = branch(Color::Red, as_black(x), v, y);
  }
  return t;
}
auto TreeBase::join_right(const Ptr &x, const PairType &v, const Ptr &y)
    -> Ptr {
  if (rank(y) < rank(x)) {
    const auto w = x->value();
    const auto l = x->left();
    const auto z = join_right(x->right(), v, y);
    if (is_red_twice(z)) {
      if (is_black(l)) {
        const auto t = branch(Color::Red, l, w, z->left());
        return branch(Color::Black, t, z->value(), z->right());
      } else {
        return branch(Color::Red, as_black(l), w, as_black(z));
      }
    }
    return branch(x->color(), l, w, z);
  }
  return branch(Color::Red, x, v, as_black(y));
}
std::size_t TreeBase::calc_rank(const Ptr &x) {
  return rank(x) + (is_black(x) ? 1 : 0);
}
std::size_t TreeBase::calc_size(const Ptr &x, const Ptr &y) {
  return size(x) + size(y) + 1;
}

}  // namespace rbtree
}  // namespace stacksafe
