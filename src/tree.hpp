#ifndef INCLUDE_GUARD_6F2C02D5_616D_48CD_8A43_B837E2ADE127
#define INCLUDE_GUARD_6F2C02D5_616D_48CD_8A43_B837E2ADE127

#include <memory>
#include "node.hpp"

namespace stacksafe {
class NodeSet;
namespace rbtree {
class TreeBase;
using PairType = std::tuple<const Node, const Node>;
using TreePtr = std::shared_ptr<const TreeBase>;

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

class TreeBase : private Header {
  using Ptr = std::shared_ptr<const TreeBase>;
  using Result = std::tuple<Ptr, bool, Ptr>;
  const Ptr left_, right_;
  PairType value_;

 public:
  TreeBase(const Ptr &l, const Ptr &r, bool b, PairType v);
  using Header::rank, Header::size, Header::is_black;
  static Ptr single(PairType v);
  const Ptr left() const;
  const Ptr right() const;
  const PairType &value() const;
  static int get_size(const Ptr &x);
  static Ptr join(const Ptr &x, PairType v, const Ptr &y);
  static Result split(const Ptr &x, PairType v);

 private:
  template <typename S>
  struct MakeSharedHelper : public S {
    template <typename... T>
    explicit MakeSharedHelper(T &&... args) : S{std::forward<T>(args)...} {}
  };
  using Helper = MakeSharedHelper<TreeBase>;
  static Ptr branch(const Ptr &l, const Ptr &r, bool b, PairType v);
  static Ptr branch(const Ptr &l, const Ptr &c, const Ptr &r, bool b);
  static Ptr red(const Ptr &l, const Ptr &c, const Ptr &r);
  static Ptr red(const Ptr &l, PairType v, const Ptr &r);
  static Ptr black(const Ptr &l, const Ptr &c, const Ptr &r);
  static Ptr black(const Ptr &x);
  static Ptr as_root(const Ptr &x);
  static void find(const Ptr &x, const Node &key, NodeSet &values);
  static bool is_black(const Ptr &x);
  static bool is_red_twice(const Ptr &x);
  static bool is_valid(const Ptr &x);
  static Ptr join_left(const Ptr &x, PairType v, const Ptr &y);
  static Ptr join_right(const Ptr &x, PairType v, const Ptr &y);
  static int get_rank(const Ptr &x);
  static bool less_rank(const Ptr &x, const Ptr &y);

 private:
  static std::size_t calc_rank(const Ptr &x);
  static std::size_t calc_size(const Ptr &x, const Ptr &y);
};

}  // namespace rbtree

using TreePtr = std::shared_ptr<const rbtree::TreeBase>;
}  // namespace stacksafe

#endif  // INCLUDE_GUARD_6F2C02D5_616D_48CD_8A43_B837E2ADE127
