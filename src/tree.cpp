#include "tree.hpp"

namespace tree {

Node::Node(Ptr l, Ptr r, bool b, int k, int s, int v)
    : left_{l}, right_{r}, black_{b}, rank_{k}, size_{s}, value_{v} {}

}  // namespace tree
