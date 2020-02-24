#ifndef INCLUDE_GUARD_AFF58DC7_979A_4BED_A89B_F759B72BD2AE
#define INCLUDE_GUARD_AFF58DC7_979A_4BED_A89B_F759B72BD2AE

#include <tuple>
#include <vector>
#include "map.hpp"

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

class Components : private std::vector<std::tuple<Blocks, Map>> {
  using Pair = std::tuple<Blocks, Map>;
  using Super = std::vector<Pair>;
  using BB = Blocks::value_type;
  friend class Tarjan;

 public:
  using Super::begin, Super::end;
  void transfer(const Blocks& b, const Map& pred);

 private:
  Map& find(BB b);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_AFF58DC7_979A_4BED_A89B_F759B72BD2AE
