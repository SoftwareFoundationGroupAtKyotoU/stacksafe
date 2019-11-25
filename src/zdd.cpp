#include "zdd.hpp"

namespace stacksafe {

Node::Node(Pair top, NodePtr lo, NodePtr hi)
    : top_{top}, lo_{std::move(lo)}, hi_{std::move(hi)} {}
bool Node::is_top() const {
  return top_ == Pair::get_negative() && !lo_ && !hi_;
}
bool Node::is_bot() const {
  return top_ == Pair::get_zero() && !lo_ && !hi_;
}
NodePtr Node::make(Pair top, NodePtr lo, NodePtr hi) {
  return std::shared_ptr<Node>{new Node{top, std::move(lo), std::move(hi)}};
}
NodePtr Node::get_top() {
  return Node::make(Pair::get_negative(), nullptr, nullptr);
}
NodePtr Node::get_bot() {
  return Node::make(Pair::get_zero(), nullptr, nullptr);
}

Zdd::Zdd() : root_{Node::get_bot()} {}
Zdd::Zdd(const std::set<Pair>& pairs) : root_{Node::get_top()} {
  auto bot = Node::get_bot();
  for (const auto& pair : pairs) {
    root_ = Node::make(pair, bot, root_);
  }
}

}  // namespace stacksafe
