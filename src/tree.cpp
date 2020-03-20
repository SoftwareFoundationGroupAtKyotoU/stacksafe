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

Tree::Tree(const Ptr &l, const Ptr &r, Color c, const PairType &v)
    : Header{c, calc_rank(l), calc_size(l, r)},
      left_{l},
      right_{r},
      value_{v} {}
auto Tree::make(const Node &k, const Node &v) -> Ptr {
  return red(nullptr, std::make_tuple(k, v), nullptr);
}
auto Tree::left() const -> const Ptr {
  return left_;
}
auto Tree::right() const -> const Ptr {
  return right_;
}
auto Tree::color() const -> Color {
  return Header::is_black() ? Color::Black : Color::Red;
}
auto Tree::value() const -> const PairType & {
  return value_;
}
std::size_t Tree::size(const Ptr &x) {
  return x ? x->Header::size() : 0;
}
std::size_t Tree::rank(const Ptr &x) {
  return x ? x->Header::rank() : 0;
}
auto Tree::join(const Ptr &x, const PairType &v, const Ptr &y) -> Ptr {
  if (rank(x) == rank(y)) {
    return (is_black(x) || is_black(y)) ? red(as_black(x), v, as_black(y)) :
                                          black(x, v, y);
  } else {
    const auto z =
        (rank(x) < rank(y)) ? join_left(x, v, y) : join_right(x, v, y);
    return is_red_twice(z) ? as_black(z) : z;
  }
}
auto Tree::split(const Ptr &x, const PairType &v) -> Result {
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

auto Tree::branch(Color c, const Ptr &l, const PairType &v, const Ptr &r)
    -> Ptr {
  const auto t = std::make_shared<Helper>(l, r, c, v);
  assert(is_valid(t, false));
  return t;
}
auto Tree::black(const Ptr &l, const PairType &v, const Ptr &r) -> Ptr {
  const auto t = std::make_shared<Helper>(l, r, Color::Black, v);
  assert(is_valid(t, true));
  return t;
}
auto Tree::red(const Ptr &l, const PairType &v, const Ptr &r) -> Ptr {
  const auto t = std::make_shared<Helper>(l, r, Color::Red, v);
  assert(is_valid(t, true));
  return t;
}
auto Tree::as_black(const Ptr &x) -> Ptr {
  return is_black(x) ? x : black(x->left(), x->value(), x->right());
}
bool Tree::is_black(const Ptr &x) {
  return x ? x->Header::is_black() : true;
}
bool Tree::is_red_twice(const Ptr &x) {
  return !is_black(x) && !(is_black(x->left()) && is_black(x->right()));
}
auto Tree::join_left(const Ptr &x, const PairType &v, const Ptr &y) -> Ptr {
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
      assert(is_black(z->right()));
      if (is_black(r)) {
        t = red(z->right(), w, r);
        t = black(z->left(), z->value(), t);
      } else {
        t = red(as_black(z), w, as_black(r));
      }
    } else {
      t = branch(y->color(), z, w, r);
      assert(is_black(y) || is_black(z) || is_red_twice(t));
    }
  } else {
    assert(is_black(y));
    t = red(as_black(x), v, y);
  }
  return t;
}
auto Tree::join_right(const Ptr &x, const PairType &v, const Ptr &y) -> Ptr {
  if (rank(y) < rank(x)) {
    const auto w = x->value();
    const auto l = x->left();
    const auto z = join_right(x->right(), v, y);
    if (is_red_twice(z)) {
      if (is_black(l)) {
        const auto t = red(l, w, z->left());
        return black(t, z->value(), z->right());
      } else {
        return red(as_black(l), w, as_black(z));
      }
    }
    return branch(x->color(), l, w, z);
  }
  return red(x, v, as_black(y));
}
std::size_t Tree::calc_rank(const Ptr &x) {
  return rank(x) + (is_black(x) ? 1 : 0);
}
std::size_t Tree::calc_size(const Ptr &x, const Ptr &y) {
  return size(x) + size(y) + 1;
}
bool Tree::is_value_valid(const Ptr &x) {
  return x ? ((x->left() ? x->left()->value() < x->value() : true) &&
              (x->right() ? x->value() < x->right()->value() : true)) :
             true;
}
bool Tree::is_rank_valid(const Ptr &x) {
  return x ? (rank(x) == calc_rank(x->left()) &&
              rank(x) == calc_rank(x->right())) :
             true;
}
bool Tree::is_size_valid(const Ptr &x) {
  return x ? size(x) == size(x->left()) + size(x->right()) + 1 : true;
}
bool Tree::is_color_valid(const Ptr &x) {
  return is_black(x) ? true : is_black(x->left()) && is_black(x->right());
}
bool Tree::is_valid(const Ptr &x, bool color_check) {
  return x ? (is_valid(x->left(), color_check) &&
              is_valid(x->right(), color_check) && is_value_valid(x) &&
              is_rank_valid(x) && is_size_valid(x) &&
              (!color_check || is_color_valid(x))) :
             true;
}

}  // namespace rbtree
}  // namespace stacksafe
