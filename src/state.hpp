#ifndef INCLUDE_GUARD_F20B95D3_9559_4A85_99D6_241914BC4F14
#define INCLUDE_GUARD_F20B95D3_9559_4A85_99D6_241914BC4F14

#include "block.hpp"
#include "graph.hpp"

namespace stacksafe {

class Component {
  friend class State;
  Blocks blocks_;
  Graph graph_;

 public:
  explicit Component(const Blocks& b);

 private:
  bool check_global() const;
};

class State : private std::vector<Component> {
  using Super = std::vector<Component>;
  using BB = Blocks::value_type;

 public:
  using Super::begin, Super::end;
  void transfer(const Component& c);

 private:
  Graph& find(BB b);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_F20B95D3_9559_4A85_99D6_241914BC4F14
