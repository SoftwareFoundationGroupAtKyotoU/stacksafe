#ifndef INCLUDE_GUARD_F20B95D3_9559_4A85_99D6_241914BC4F14
#define INCLUDE_GUARD_F20B95D3_9559_4A85_99D6_241914BC4F14

#include "block.hpp"
#include "graph.hpp"

namespace stacksafe {

class Component {
  friend class State;
  using BB = Blocks::value_type;
  const Blocks blocks_;
  Graph graph_;

 public:
  explicit Component(const Blocks& b);
  const Blocks& blocks() const;
  Graph& graph();
  bool is_safe() const;

 private:
  bool check_global() const;
  bool check_return(BB b) const;
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
