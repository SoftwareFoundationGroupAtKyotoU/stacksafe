#include "node.hpp"
#include <llvm/IR/Instructions.h>
#include <llvm/Support/ErrorHandling.h>
#include "utility.hpp"

namespace stacksafe {

Node Node::get_constant() {
  return Node{Constant{nullptr}};
}
Node Node::get_global() {
  return Node{Global{nullptr}};
}
Node Node::get_local(const llvm::AllocaInst &l) {
  return Node{Local{&l}};
}
Node Node::get_register(const llvm::Argument &a) {
  return Node{Argument{&a}};
}
Node Node::get_register(const llvm::Instruction &i) {
  return Node{Register{&i}};
}
Node Node::from_value(const llvm::Value &v) {
  if (auto c = llvm::dyn_cast<llvm::Constant>(&v)) {
    return is_global(*c) ? Node::get_global() : Node::get_constant();
  } else if (auto i = llvm::dyn_cast<llvm::Instruction>(&v)) {
    assert(is_register(*i) && "invalid register lookup");
    return Node::get_register(*i);
  } else if (auto a = llvm::dyn_cast<llvm::Argument>(&v)) {
    return Node::get_register(*a);
  } else {
    llvm_unreachable("invalid value lookup");
  };
}
std::pair<std::size_t, const void *> Node::pair() const {
  return {var_.index(), ptr()};
}
bool Node::is_symbol() const {
  return std::get_if<Global>(&var_) || std::get_if<Local>(&var_);
}
bool Node::is_local() const {
  return std::get_if<Local>(&var_);
}
const void *Node::ptr() const {
  const auto f = [](auto &&v) -> const void * { return v.get(); };
  return std::visit(f, var_);
}
bool operator==(const Node &lhs, const Node &rhs) {
  return lhs.pair() == rhs.pair();
}
bool operator<(const Node &lhs, const Node &rhs) {
  return lhs.pair() < rhs.pair();
}

void NodeSet::merge(const NodeSet &nodes) {
  Super::insert(nodes.begin(), nodes.end());
}
bool NodeSet::element(const Node &n) const {
  return 0 != Super::count(n);
}
bool NodeSet::has_local() const {
  for (const auto &n : *this) {
    if (n.is_local()) {
      return true;
    }
  }
  return false;
}

}  // namespace stacksafe
