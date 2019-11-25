#ifndef INCLUDE_GUARD_6F13AE33_4EC4_4948_AE33_FF20C0E66402
#define INCLUDE_GUARD_6F13AE33_4EC4_4948_AE33_FF20C0E66402

#include <memory>
#include <set>
#include "value.hpp"

namespace stacksafe {
class Node;
using NodePtr = std::shared_ptr<Node>;

class Node {
  const Pair label_;
  NodePtr lo_, hi_;
  Node(const Pair& pair, NodePtr lo, NodePtr hi);

 public:
  const Pair& label() const;
  bool is_top() const;
  bool is_bot() const;
  static NodePtr make(const Pair& pair, NodePtr lo, NodePtr hi);
  static NodePtr get_top();
  static NodePtr get_bot();
};

class Zdd {
  NodePtr root_;

 public:
  Zdd();
  explicit Zdd(const std::set<Pair>& pairs);
  bool empty() const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_6F13AE33_4EC4_4948_AE33_FF20C0E66402
