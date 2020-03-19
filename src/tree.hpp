#ifndef INCLUDE_GUARD_6F2C02D5_616D_48CD_8A43_B837E2ADE127
#define INCLUDE_GUARD_6F2C02D5_616D_48CD_8A43_B837E2ADE127

#include <memory>
#include "node.hpp"

namespace stacksafe {
class NodeSet;
class RedBlackTree;
using PairType = std::tuple<Node, Node>;
using TreePtr = std::shared_ptr<const RedBlackTree>;

class Header {
  const std::size_t rank_, size_;
  const TreePtr left_, right_;
  using Tree = RedBlackTree;

 public:
  Header(const TreePtr &x, const TreePtr &y, bool b, std::size_t k,
         std::size_t s);
  std::size_t rank() const;
  std::size_t size() const;
  bool is_black() const;
  const TreePtr left() const;
  const TreePtr right() const;

 private:
  static std::size_t combine(bool b, std::size_t k);
  static std::size_t calc_rank(const TreePtr &x, const TreePtr &y);
  static std::size_t calc_size(const TreePtr &x, const TreePtr &y);
};

class RedBlackTree : private Header {
  using Ptr = std::shared_ptr<const RedBlackTree>;
  using Result = std::tuple<Ptr, bool, Ptr>;
  PairType value_;
  RedBlackTree(const Ptr &l, const Ptr &r, bool b, PairType v);
  using Header::rank, Header::size, Header::is_black, Header::left,
      Header::right;

 public:
  static Ptr leaf();
  static Ptr single(PairType v);
  static Ptr branch(const Ptr &l, const Ptr &r, bool b, PairType v);
  static Ptr as_root(const Ptr &x);
  static bool exists(const Ptr &x, PairType v);
  static Ptr merge(const Ptr &x, const Ptr &y);
  static NodeSet find(const Ptr &x, const Node &key);
  static int get_size(const Ptr &x);

  static void find(const Ptr &x, const Node &key, NodeSet &values);
  static bool is_black(const Ptr &x);
  static bool is_red_twice(const Ptr &x);
  static bool is_valid(const Ptr &x);
  static Ptr branch(const Ptr &l, const Ptr &c, const Ptr &r, bool b);
  static Ptr red(const Ptr &l, const Ptr &c, const Ptr &r);
  static Ptr red(const Ptr &l, PairType v, const Ptr &r);
  static Ptr black(const Ptr &l, const Ptr &c, const Ptr &r);
  static Ptr black(const Ptr &x);
  static Ptr join_left(const Ptr &x, PairType v, const Ptr &y);
  static Ptr join_right(const Ptr &x, PairType v, const Ptr &y);
  static Ptr join(const Ptr &x, PairType v, const Ptr &y);
  static Result split(const Ptr &x, PairType v);
  static int get_rank(const Ptr &x);
  static bool less_rank(const Ptr &x, const Ptr &y);

 private:
  template <typename S>
  struct MakeSharedHelper : public S {
    template <typename... T>
    explicit MakeSharedHelper(T &&... args) : S{std::forward<T>(args)...} {}
  };
  using Helper = MakeSharedHelper<RedBlackTree>;
  const PairType &value() const;

 private:
  static std::size_t calc_rank(const Ptr &x);
  static std::size_t calc_size(const Ptr &x, const Ptr &y);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_6F2C02D5_616D_48CD_8A43_B837E2ADE127
