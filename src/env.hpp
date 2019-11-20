#ifndef INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA
#define INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA

#include <unordered_set>
#include "map.hpp"

namespace llvm {
class Function;
}  // namespace llvm

namespace stacksafe {
class Map;

class FlatEnv {
  Map heap_, stack_;

 public:
  explicit FlatEnv(const llvm::Function &f);
  FlatEnv(const Map &heap, const Map &stack);
  const Map &heap() const;
  const Map &stack() const;
};

class Env {
  std::unordered_set<MapRef> heap_, stack_;

 public:
  Env() = default;
  Env(MapRef heap, MapRef stack);
  Map heap() const;
  Map stack() const;
  void merge(const Env &env);
  bool includes(const Env &env);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA
