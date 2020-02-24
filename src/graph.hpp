#ifndef INCLUDE_GUARD_E1E5ACD3_3435_4167_A6B2_0E8D6A2A38AC
#define INCLUDE_GUARD_E1E5ACD3_3435_4167_A6B2_0E8D6A2A38AC

#include <vector>
#include "node.hpp"

namespace stacksafe {

class Graph : private std::vector<Edge> {
  using Super = std::vector<Edge>;
  using iterator = Super::iterator;
  using Result = std::tuple<iterator, bool>;

 private:
  Result insert(iterator hint, const Edge& e);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_E1E5ACD3_3435_4167_A6B2_0E8D6A2A38AC
