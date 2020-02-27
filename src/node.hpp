#ifndef INCLUDE_GUARD_A9F529C8_A504_4663_8ADD_A1FC022EA7B5
#define INCLUDE_GUARD_A9F529C8_A504_4663_8ADD_A1FC022EA7B5

#include <set>
#include <variant>
#include "value.hpp"

namespace llvm {
class AllocaInst;
class Argument;
class Instruction;
class Value;
}  // namespace llvm

namespace stacksafe {
namespace {
using Constant = Value<int>;
using Global = Value<void>;
using Local = Value<llvm::AllocaInst>;
using Register = Value<llvm::Instruction>;
using Argument = Value<llvm::Argument>;
using Variant = std::variant<Constant, Global, Local, Register, Argument>;
}  // namespace

class Node : private Variant {
  using Variant::Variant;

 public:
  static Node get_constant();
  static Node get_global();
  static Node get_local(const llvm::AllocaInst &l);
  static Node get_register(const llvm::Argument &a);
  static Node get_register(const llvm::Instruction &i);
  static Node from_value(const llvm::Value &v);
  const void *ptr() const;
  std::pair<std::size_t, const void *> pair() const;
  bool is_symbol() const;
  bool is_local() const;
};
bool operator==(const Node &lhs, const Node &rhs);
bool operator<(const Node &lhs, const Node &rhs);

class NodeSet : private std::set<Node> {
  using Super = std::set<Node>;

 public:
  using Super::begin, Super::end, Super::insert;
  void merge(const NodeSet &nodes);
  bool element(const Node &n) const;
  bool has_local() const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_A9F529C8_A504_4663_8ADD_A1FC022EA7B5
