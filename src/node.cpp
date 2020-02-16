#include "node.hpp"

namespace stacksafe {

Node::Node(const Symbol &sym) : Super{sym} {}
Node::Node(const Register &reg) : Super{reg} {}

}  // namespace stacksafe
