#ifndef INCLUDE_GUARD_E1E5ACD3_3435_4167_A6B2_0E8D6A2A38AC
#define INCLUDE_GUARD_E1E5ACD3_3435_4167_A6B2_0E8D6A2A38AC

#include <vector>
#include "block.hpp"
#include "map.hpp"

namespace stacksafe {

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

#endif  // INCLUDE_GUARD_E1E5ACD3_3435_4167_A6B2_0E8D6A2A38AC
