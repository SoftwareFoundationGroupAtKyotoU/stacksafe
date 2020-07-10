#ifndef INCLUDE_GUARD_AFF58DC7_979A_4BED_A89B_F759B72BD2AE
#define INCLUDE_GUARD_AFF58DC7_979A_4BED_A89B_F759B72BD2AE

#include <tuple>
#include <vector>
#include "tarjan.hpp"

namespace llvm {
class BasicBlock;
}

namespace stacksafe {

class Blocks : private std::vector<const llvm::BasicBlock*> {
  using Super = std::vector<const llvm::BasicBlock*>;
  using BB = Super::value_type;

 public:
  using Super::begin, Super::end, Super::push_back;
  using Super::value_type;
  static Blocks successors(BB b);
  Blocks successors() const;
  bool contains(BB b) const;
  bool is_loop() const;
};

}  // namespace stacksafe

namespace dataflow {
namespace block {
using Component = std::vector<const llvm::BasicBlock*>;
class Scc : public Tarjan {
 public:
  static std::vector<Component> solve(const llvm::Function& f);

 private:
  Vec successors(Ptr p) const override;
};
}  // namespace block
}  // namespace dataflow

#endif  // INCLUDE_GUARD_AFF58DC7_979A_4BED_A89B_F759B72BD2AE
