#ifndef INCLUDE_GUARD_F20B95D3_9559_4A85_99D6_241914BC4F14
#define INCLUDE_GUARD_F20B95D3_9559_4A85_99D6_241914BC4F14

#include <vector>
#include "block.hpp"
#include "component.hpp"

namespace llvm {
class Function;
}

namespace stacksafe {

class State : private std::vector<Component> {
  using Super = std::vector<Component>;
  using BB = Blocks::value_type;

 public:
  using Super::begin, Super::end;
  explicit State(const llvm::Function &f);
  void transfer(const Component &c);
  bool is_safe() const;

 private:
  Component &find(BB b);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_F20B95D3_9559_4A85_99D6_241914BC4F14
