#include "zdd.hpp"

namespace stacksafe {

Node::Node(const Pair& pair, NodePtr lo, NodePtr hi)
    : label_{pair}, lo_{std::move(lo)}, hi_{std::move(hi)} {}
const Pair& Node::label() const {
  return label_;
}
bool Node::is_top() const {
  return label_ == Pair::get_negative() && !lo_ && !hi_;
}
bool Node::is_bot() const {
  return label_ == Pair::get_zero() && !lo_ && !hi_;
}
bool Node::equals(const NodePtr& lhs, const NodePtr& rhs) {
  if (lhs && rhs) {
    return lhs->label_ == rhs->label_ && equals(lhs->lo_, rhs->lo_) &&
           equals(lhs->hi_, rhs->hi_);
  } else {
    return !lhs && !rhs;
  }
}
NodePtr Node::make(const Pair& pair, NodePtr lo, NodePtr hi) {
  return std::shared_ptr<Node>{new Node{pair, std::move(lo), std::move(hi)}};
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
bool Zdd::empty() const {
  return root_->is_bot();
}

}  // namespace stacksafe
