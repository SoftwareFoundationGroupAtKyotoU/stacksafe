#ifndef INCLUDE_GUARD_6831D829_3A6A_4EE8_ACCB_2E3598692958
#define INCLUDE_GUARD_6831D829_3A6A_4EE8_ACCB_2E3598692958

#include <set>
#include "block.hpp"
#include "graph.hpp"

namespace llvm {
class Function;
class Value;
}  // namespace llvm

namespace stacksafe {

class Component {
  using BB = Blocks::value_type;
  const Blocks blocks_;
  Graph graph_;
  std::set<const Graph*> preds_;

 public:
  explicit Component(const Blocks& b);
  const Blocks& blocks() const;
  Graph& graph();
  bool is_safe() const;
  void add_pred(const Component& c);
  void init(const llvm::Function& f);
  bool contains(const Node& tail, const Node& head) const;
  bool contains(const llvm::Value& tail, const Node& head) const;
  void connect(const NodeSet& tails, const NodeSet& heads);
  void connect(const llvm::Value& tail, const NodeSet& heads);
  void followings(const NodeSet& tails, NodeSet& heads) const;
  void followings(const llvm::Value& tail, NodeSet& heads) const;
  NodeSet reachables(const Node& tail) const;
  NodeSet reachables(const llvm::Value& tail) const;

 private:
  bool check_global() const;
  bool check_return(BB b) const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_6831D829_3A6A_4EE8_ACCB_2E3598692958
