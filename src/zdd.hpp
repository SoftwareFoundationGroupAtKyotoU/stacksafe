#ifndef INCLUDE_GUARD_6F13AE33_4EC4_4948_AE33_FF20C0E66402
#define INCLUDE_GUARD_6F13AE33_4EC4_4948_AE33_FF20C0E66402

#include <memory>
#include "value.hpp"

namespace stacksafe {
class Node;
using NodePtr = std::unique_ptr<Node>;

class Node {
  Pair level_;
  NodePtr lo_, hi_;
};

class Zdd {
  NodePtr root_;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_6F13AE33_4EC4_4948_AE33_FF20C0E66402
