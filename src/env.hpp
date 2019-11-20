#ifndef INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA
#define INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA

#include <unordered_set>
#include "flat.hpp"

namespace llvm {
class Function;
}  // namespace llvm

namespace stacksafe {
class MultiMap;

class FlatEnv {
  FlatMap heap_, stack_;

 public:
  explicit FlatEnv(const llvm::Function &f);
  FlatEnv(const FlatMap &heap, const FlatMap &stack);
  const FlatMap &heap() const;
  const FlatMap &stack() const;
};

class Env {
  std::unordered_set<FlatMapRef> heap_, stack_;

 public:
  Env() = default;
  Env(FlatMapRef heap, FlatMapRef stack);
  MultiMap heap() const;
  MultiMap stack() const;
  void merge(const Env &env);
  bool includes(const Env &env);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA
