#ifndef INCLUDE_GUARD_6F2C02D5_616D_48CD_8A43_B837E2ADE127
#define INCLUDE_GUARD_6F2C02D5_616D_48CD_8A43_B837E2ADE127

#include <cassert>
#include <memory>

namespace stacksafe {

class Header {
  const std::size_t rank_, size_;

 public:
  enum class Color : std::size_t { Red, Black };
  Header(Color c, std::size_t k, std::size_t s);
  std::size_t rank() const;
  std::size_t size() const;
  bool is_black() const;

 private:
  static std::size_t combine(Color c, std::size_t k);
};

template <typename Key, typename Val>
class Tree : private Header {
 public:
  using Ptr = std::shared_ptr<const Tree>;
  using PairType = std::tuple<const Key, const Val>;
  using Triple = std::tuple<Ptr, bool, Ptr>;
  const PairType &value() const { return value_; }
  const Ptr left() const { return left_; }
  const Ptr right() const { return right_; }
  Color color() const { return Header::is_black() ? Color::Black : Color::Red; }
  static Ptr make(const Key &k, const Val &v) {
    return red(nullptr, std::make_tuple(k, v), nullptr);
  }
  static std::size_t size(const Ptr &x) { return x ? x->Header::size() : 0; }
  static std::size_t rank(const Ptr &x) { return x ? x->Header::rank() : 0; }
  static Ptr join(const Ptr &x, const PairType &v, const Ptr &y) {
    if (rank(x) == rank(y)) {
      return (is_black(x) || is_black(y)) ? red(as_black(x), v, as_black(y)) :
                                            black(x, v, y);
    } else {
      const auto z =
          (rank(x) < rank(y)) ? join_left(x, v, y) : join_right(x, v, y);
      return is_red_twice(z) ? as_black(z) : z;
    }
  }
  static Triple split(const Ptr &x, const PairType &v) {
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

 private:
  Tree(const Ptr &l, const Ptr &r, Color c, const PairType &v)
      : Header{c, calc_rank(l), calc_size(l, r)},
        left_{l},
        right_{r},
        value_{v} {}

  static Ptr branch(Color c, const Ptr &l, const PairType &v, const Ptr &r) {
    const auto t = std::make_shared<Helper>(l, r, c, v);
    assert(is_red_twice(t) || is_valid(t, false));
    return t;
  }
  static Ptr black(const Ptr &l, const PairType &v, const Ptr &r) {
    const auto t = std::make_shared<Helper>(l, r, Color::Black, v);
    assert(is_valid(t, true));
    return t;
  }
  static Ptr red(const Ptr &l, const PairType &v, const Ptr &r) {
    const auto t = std::make_shared<Helper>(l, r, Color::Red, v);
    assert(is_valid(t, true));
    return t;
  }
  static Ptr as_black(const Ptr &x) {
    return is_black(x) ? x : black(x->left(), x->value(), x->right());
  }
  static bool is_black(const Ptr &x) {
    return x ? x->Header::is_black() : true;
  }
  static bool is_red_twice(const Ptr &x) {
    return !is_black(x) && !(is_black(x->left()) && is_black(x->right()));
  }
  static Ptr join_left(const Ptr &x, const PairType &v, const Ptr &y) {
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
  static Ptr join_right(const Ptr &x, const PairType &v, const Ptr &y) {
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
  static std::size_t calc_rank(const Ptr &x) {
    return rank(x) + (is_black(x) ? 1 : 0);
  }
  static std::size_t calc_size(const Ptr &x, const Ptr &y) {
    return size(x) + size(y) + 1;
  }
  static bool is_value_valid(const Ptr &x) {
    return x ? ((x->left() ? x->left()->value() < x->value() : true) &&
                (x->right() ? x->value() < x->right()->value() : true)) :
               true;
  }
  static bool is_rank_valid(const Ptr &x) {
    return x ? (rank(x) == calc_rank(x->left()) &&
                rank(x) == calc_rank(x->right())) :
               true;
  }
  static bool is_size_valid(const Ptr &x) {
    return x ? size(x) == size(x->left()) + size(x->right()) + 1 : true;
  }
  static bool is_color_valid(const Ptr &x) {
    return is_black(x) ? true : is_black(x->left()) && is_black(x->right());
  }
  static bool is_valid(const Ptr &x, bool color_check) {
    return x ? (is_valid(x->left(), color_check) &&
                is_valid(x->right(), color_check) && is_value_valid(x) &&
                is_rank_valid(x) && is_size_valid(x) &&
                (!color_check || is_color_valid(x))) :
               true;
  }

 private:
  template <typename S>
  struct MakeSharedHelper : public S {
    template <typename... T>
    explicit MakeSharedHelper(T &&... args) : S{std::forward<T>(args)...} {}
  };
  using Helper = MakeSharedHelper<Tree>;
  const Ptr left_, right_;
  PairType value_;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_6F2C02D5_616D_48CD_8A43_B837E2ADE127
