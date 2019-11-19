#ifndef INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA
#define INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA

#include <unordered_set>
#include "flat.hpp"
#include "map.hpp"

namespace llvm {
class Function;
}  // namespace llvm

namespace stacksafe {
class FlatEnv;
class FlatMapRef;
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
  FlatEnv to_flat_env() const;
};

class EnvOld {
  std::unordered_set<MapRef> heap_, stack_;

 public:
  EnvOld() = default;
  EnvOld(MapRef heap, MapRef stack);
  void merge(const EnvOld &env);
  FlatEnvOld concat() const;
};

class FlatEnv {
  FlatMap heap_, stack_;

 public:
  explicit FlatEnv(const llvm::Function &f);
  FlatEnv(const FlatMap &heap, const FlatMap &stack);
  const FlatMap &heap() const;
  FlatMap &heap();
  const FlatMap &stack() const;
  FlatMap &stack();
  bool includes(const FlatEnv &flat) const;
};

class Env {
  std::unordered_set<FlatMapRef> heap_, stack_;

 public:
  Env() = default;
  Env(FlatMapRef heap, FlatMapRef stack);
  void merge(const Env &env);
  FlatEnvOld concat() const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA
