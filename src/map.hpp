#ifndef INCLUDE_GUARD_B24913BE_A7FB_43A0_9D42_907C04FE896D
#define INCLUDE_GUARD_B24913BE_A7FB_43A0_9D42_907C04FE896D

#include "tree.hpp"

namespace stacksafe {
class NodeSet;

class Map {
  TreePtr tree_;

 public:
  void add(const Node& k, const Node& v);
  bool exists(const Node& k, const Node& v) const;
  NodeSet lookup(const Node& k) const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_B24913BE_A7FB_43A0_9D42_907C04FE896D
