#ifndef INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA
#define INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA

#include <unordered_set>
#include "map.hpp"

namespace stacksafe {

class Env {
  std::unordered_set<MapRef> heap_, stack_;

 public:
  Env() = default;
  Env(MapRef heap, MapRef stack);
  Map heap() const;
  Map stack() const;
  void merge(const Env &env);
  void merge_heap(MapRef ref);
  void merge_stack(MapRef ref);
  bool includes(const Env &env);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA
