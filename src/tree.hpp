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

 private:
  template <typename S>
  struct MakeSharedHelper : public S {
    template <typename... T>
    explicit MakeSharedHelper(T &&... args) : S{std::forward<T>(args)...} {}
  };
  using Helper = MakeSharedHelper<Node>;
};

}  // namespace tree

#endif  // INCLUDE_GUARD_6F2C02D5_616D_48CD_8A43_B837E2ADE127
