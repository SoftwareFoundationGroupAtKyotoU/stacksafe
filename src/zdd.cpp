#include "zdd.hpp"

namespace stacksafe {

Node::Node(Pair top, NodePtr lo, NodePtr hi)
    : top_{top}, lo_{std::move(lo)}, hi_{std::move(hi)} {}
NodePtr Node::make(Pair top, NodePtr lo, NodePtr hi) {
  return std::unique_ptr<Node>{new Node{top, std::move(lo), std::move(hi)}};
}
NodePtr Node::zero() {
  return Node::make(Pair::get_zero(), nullptr, nullptr);
}
NodePtr Node::get_top() {
  return Node::make(Pair::get_negative(), nullptr, nullptr);
}
NodePtr Node::get_bot() {
  return Node::make(Pair::get_zero(), nullptr, nullptr);
}

}  // namespace stacksafe
