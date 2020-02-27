#ifndef INCLUDE_GUARD_F20B95D3_9559_4A85_99D6_241914BC4F14
#define INCLUDE_GUARD_F20B95D3_9559_4A85_99D6_241914BC4F14

#include "block.hpp"
#include "graph.hpp"

namespace stacksafe {

class Component {
  Blocks blocks_;
  Graph graph_;

 public:
  explicit Component(const Blocks& b);
};

class State : private std::vector<Component> {
  using Super = std::vector<Component>;

 public:
  using Super::begin, Super::end;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_F20B95D3_9559_4A85_99D6_241914BC4F14
