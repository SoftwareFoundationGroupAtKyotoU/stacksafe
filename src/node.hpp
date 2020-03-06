#ifndef INCLUDE_GUARD_A9F529C8_A504_4663_8ADD_A1FC022EA7B5
#define INCLUDE_GUARD_A9F529C8_A504_4663_8ADD_A1FC022EA7B5

#include <utility>

namespace llvm {
class AllocaInst;
class Value;
}  // namespace llvm

namespace stacksafe {

class Node {
  enum class Kind : std::size_t {
    Constant,
    Global,
    Local,
    Register,
    Argument,
  } kind_;
  const void *ptr_;
  Node(Kind k, const void *p);

 public:
  static Node get_constant();
  static Node get_global();
  static Node get_local(const llvm::AllocaInst &l);
  bool is_reg() const;
  bool is_symbol() const;
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
