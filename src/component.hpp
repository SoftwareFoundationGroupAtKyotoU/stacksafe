#ifndef INCLUDE_GUARD_6831D829_3A6A_4EE8_ACCB_2E3598692958
#define INCLUDE_GUARD_6831D829_3A6A_4EE8_ACCB_2E3598692958

#include <vector>
#include "block.hpp"
#include "graph.hpp"

namespace stacksafe {

class Component {
  using BB = Blocks::value_type;
  const Blocks blocks_;
  Graph graph_;
  std::vector<const Graph*> preds_;

 public:
  explicit Component(const Blocks& b);
  const Blocks& blocks() const;
  Graph& graph();
  bool is_safe() const;
  void add_pred(const Graph& g);
  void transfer();

 private:
  bool check_global() const;
  bool check_return(BB b) const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_6831D829_3A6A_4EE8_ACCB_2E3598692958
