#ifndef INCLUDE_GUARD_A9F529C8_A504_4663_8ADD_A1FC022EA7B5
#define INCLUDE_GUARD_A9F529C8_A504_4663_8ADD_A1FC022EA7B5

#include <cstdint>

namespace llvm {
class AllocaInst;
}  // namespace llvm

namespace stacksafe {

class Node {
  enum class Kind : std::size_t {
    Local,
    Global,
  } kind_;
  union {
    const llvm::AllocaInst *ptr_;
    std::uintptr_t val_;
  };
  Node(Kind k, const llvm::AllocaInst *p);
  explicit Node(std::uintptr_t v);

 public:
  static Node get_global();
  static Node get_local(const llvm::AllocaInst &l);
  bool is_global() const;
  bool is_local() const;
  bool equals(const Node &that) const;
  bool less(const Node &that) const;
  std::size_t hash() const;
};
bool operator==(const Node &lhs, const Node &rhs);
bool operator<(const Node &lhs, const Node &rhs);

struct NodeHash {
  std::size_t operator()(const Node &n) const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_A9F529C8_A504_4663_8ADD_A1FC022EA7B5
