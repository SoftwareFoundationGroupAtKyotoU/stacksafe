#ifndef INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA
#define INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA

#include <unordered_set>
#include "map.hpp"

namespace llvm {
class Function;
}  // namespace llvm

namespace stacksafe {
class MapRef;

class FlatEnvOld {
  Map heap_, stack_;

 public:
  FlatEnvOld() = default;
  explicit FlatEnvOld(const llvm::Function &f);
  FlatEnvOld(const Map &heap, const Map &stack);
  const Map &heap() const;
  Map &heap();
  const Map &stack() const;
  Map &stack();
  bool includes(const FlatEnvOld &that) const;
  void merge(const FlatEnvOld &that);
};

class Env {
  std::unordered_set<MapRef> heap_, stack_;

 public:
  Env() = default;
  Env(MapRef heap, MapRef stack);
  void merge(const Env &env);
  FlatEnvOld concat() const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA
