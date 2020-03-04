#include "node.hpp"
#include <llvm/IR/Instructions.h>
#include <llvm/Support/ErrorHandling.h>
#include "utility.hpp"

namespace stacksafe {

Node Node::get_constant() {
  return Node{Kind::Constant, Constant{nullptr}};
}
Node Node::get_global() {
  return Node{Kind::Global, Global{nullptr}};
}
Node Node::get_local(const llvm::AllocaInst &l) {
  return Node{Kind::Local, Local{&l}};
}
Node Node::get_register(const llvm::Argument &a) {
  return Node{Kind::Argument, Argument{&a}};
}
Node Node::get_register(const llvm::Instruction &i) {
  return Node{Kind::Register, Register{&i}};
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
  return {var_.index(), ptr_};
}
bool Node::is_reg() const {
  return kind_ == Kind::Register || kind_ == Kind::Argument;
}
bool Node::is_symbol() const {
  return !is_reg();
}
bool Node::is_local() const {
  return kind_ == Kind::Local;
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
