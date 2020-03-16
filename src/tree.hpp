#ifndef INCLUDE_GUARD_6F2C02D5_616D_48CD_8A43_B837E2ADE127
#define INCLUDE_GUARD_6F2C02D5_616D_48CD_8A43_B837E2ADE127

#include <memory>

namespace stacksafe {

class RedBlackTree {
  using Ptr = std::shared_ptr<const RedBlackTree>;
  using Result = std::tuple<Ptr, bool, Ptr>;
  Ptr left_, right_;
  bool black_;
  int rank_, size_;
  int value_;
  RedBlackTree(const Ptr &l, const Ptr &r, bool b, int k, int s, int v);

 public:
  static Ptr leaf();
  static Ptr single(int v);
  static Ptr branch(const Ptr &l, const Ptr &r, bool b, int v);
  static Ptr merge(const Ptr &x, const Ptr &y);

 private:
  static bool is_black(const Ptr &x);
  static bool is_red_twice(const Ptr &x);
  static Ptr branch(const Ptr &l, const Ptr &c, const Ptr &r, bool b);
  static Ptr red(const Ptr &l, const Ptr &c, const Ptr &r);
  static Ptr red(const Ptr &l, int v, const Ptr &r);
  static Ptr black(const Ptr &l, const Ptr &c, const Ptr &r);
  static Ptr black(const Ptr &x);
  static Ptr join_left(const Ptr &x, int v, const Ptr &y);
  static Ptr join_right(const Ptr &x, int v, const Ptr &y);
  static Ptr join(const Ptr &x, int v, const Ptr &y);
  static Result split(const Ptr &x, int v);
  static int get_rank(const Ptr &x);
  static int get_size(const Ptr &x);
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
