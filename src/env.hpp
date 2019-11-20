#ifndef INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA
#define INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA

#include <unordered_set>
#include "flat.hpp"
#include "map.hpp"

namespace llvm {
class Function;
}  // namespace llvm

namespace stacksafe {
class MultiMap;

class FlatEnv {
  MultiMap heap_, stack_;

 public:
  explicit FlatEnv(const llvm::Function &f);
  FlatEnv(const FlatMap &heap, const FlatMap &stack);
  const MultiMap &heap() const;
  const MultiMap &stack() const;
};

class Env {
  std::unordered_set<MapRef> heap_, stack_;

 public:
  Env() = default;
  Env(MapRef heap, MapRef stack);
  MultiMap heap() const;
  MultiMap stack() const;
  void merge(const Env &env);
  bool includes(const Env &env);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA
