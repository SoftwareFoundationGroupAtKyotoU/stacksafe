#ifndef INCLUDE_GUARD_6F2C02D5_616D_48CD_8A43_B837E2ADE127
#define INCLUDE_GUARD_6F2C02D5_616D_48CD_8A43_B837E2ADE127

#include <memory>
#include "node.hpp"

namespace stacksafe {
class NodeSet;
class RedBlackTree;
using PairType = std::tuple<Node, Node>;
using TreePtr = std::shared_ptr<const RedBlackTree>;

class RedBlackTree {
  using Ptr = std::shared_ptr<const RedBlackTree>;
  using Result = std::tuple<Ptr, bool, Ptr>;
  Ptr left_, right_;
  bool black_;
  int rank_, size_;
  PairType value_;
  RedBlackTree(const Ptr &l, const Ptr &r, bool b, int k, int s, PairType v);

 public:
  static Ptr leaf();
  static Ptr single(PairType v);
  static Ptr branch(const Ptr &l, const Ptr &r, bool b, PairType v);
  static Ptr as_root(const Ptr &x);
  static bool exists(const Ptr &x, PairType v);
  static Ptr merge(const Ptr &x, const Ptr &y);
  static NodeSet find(const Ptr &x, const Node &key);
  static int get_size(const Ptr &x);

 private:
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
  template <typename S>
  struct MakeSharedHelper : public S {
    template <typename... T>
    explicit MakeSharedHelper(T &&... args) : S{std::forward<T>(args)...} {}
  };
  using Helper = MakeSharedHelper<RedBlackTree>;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_6F2C02D5_616D_48CD_8A43_B837E2ADE127
