#ifndef INCLUDE_GUARD_6F2C02D5_616D_48CD_8A43_B837E2ADE127
#define INCLUDE_GUARD_6F2C02D5_616D_48CD_8A43_B837E2ADE127

#include <memory>

namespace tree {

class Node {
  using Ptr = std::shared_ptr<const Node>;
  Ptr left_, right_;
  bool black_;
  int rank_, size_;
  int value_;
  Node(Ptr l, Ptr r, bool b, int k, int s, int v);

 public:
  static Ptr leaf(int v);
  static Ptr branch(Ptr l, Ptr r, bool b);

 private:
  static Ptr red(Ptr l, Ptr r);
  static Ptr black(Ptr l, Ptr r);
  static Ptr black(Ptr x);
  static int calc_rank(const Ptr &x);
  static int calc_size(const Ptr &x, const Ptr &y);
  static bool less_rank(const Ptr &x, const Ptr &y);
  template <typename S>
  struct MakeSharedHelper : public S {
    template <typename... T>
    explicit MakeSharedHelper(T &&... args) : S{std::forward<T>(args)...} {}
  };
  using Helper = MakeSharedHelper<Node>;
};

}  // namespace tree

#endif  // INCLUDE_GUARD_6F2C02D5_616D_48CD_8A43_B837E2ADE127
