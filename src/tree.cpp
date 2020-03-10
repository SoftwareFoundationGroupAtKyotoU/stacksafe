#include "tree.hpp"
#include <cassert>

namespace tree {

Node::Node(Ptr l, Ptr r, bool b, int k, int s, int v)
    : left_{l}, right_{r}, black_{b}, rank_{k}, size_{s}, value_{v} {}
int Node::calc_rank(const Ptr &x) {
  assert(x);
  return x->rank_ + (x->black_ ? 1 : 0);
}

}  // namespace tree
