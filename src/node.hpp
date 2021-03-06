#ifndef INCLUDE_GUARD_A9F529C8_A504_4663_8ADD_A1FC022EA7B5
#define INCLUDE_GUARD_A9F529C8_A504_4663_8ADD_A1FC022EA7B5

#include <cstdint>
#include <set>

namespace llvm {
class AllocaInst;
class Value;
}  // namespace llvm

namespace stacksafe {

class Node {
  union {
    const void *ptr_;
    std::uintptr_t val_;
  };
  explicit Node(const void *p);
  explicit Node(std::uintptr_t v);

 public:
  static Node get_global();
  static Node get_argument(std::uintptr_t a);
  static Node get_local(const llvm::AllocaInst &l);
  static Node get_register(const llvm::Value &r);
  std::uintptr_t value() const;
  const void *ptr() const;
  bool is_global() const;
  bool equals(const Node &that) const;
  bool less(const Node &that) const;
  std::size_t hash() const;
};
bool operator==(const Node &lhs, const Node &rhs);
bool operator<(const Node &lhs, const Node &rhs);

struct NodeHash {
  std::size_t operator()(const Node &n) const;
};

class NodeSet : private std::set<Node> {
  using Super = std::set<Node>;

 public:
  using Super::begin, Super::end, Super::size, Super::insert;
  NodeSet();
  explicit NodeSet(const Node &n);
  void merge(const NodeSet &nodes);
  bool element(const Node &n) const;
  bool includes(const NodeSet &that) const;
  bool has_local() const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_A9F529C8_A504_4663_8ADD_A1FC022EA7B5
