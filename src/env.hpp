#ifndef INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA
#define INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA

#include <unordered_set>
#include "map.hpp"

namespace stacksafe {

class Env : private std::unordered_set<MapRef> {
  using Super = std::unordered_set<MapRef>;

 public:
  Env() = default;
  Env(MapRef ref);
  Map concat() const;
  void merge(const Env &env);
  void insert(MapRef ref);
  bool includes(const Env &env);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA
