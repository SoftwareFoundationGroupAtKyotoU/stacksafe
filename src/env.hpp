#ifndef INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA
#define INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA

#include <unordered_set>
#include "map.hpp"

namespace llvm {
class Function;
}  // namespace llvm

namespace stacksafe {
class Register;

class EnvSlice {
  friend class Env;
  Map heap_, stack_;

 public:
  EnvSlice() = default;
  explicit EnvSlice(const llvm::Function &f);
  EnvSlice(const Map &heap, const Map &stack);
  const Map &heap() const;
  const Map &stack() const;
  bool includes(const EnvSlice &that) const;
  void merge(const EnvSlice &that);
  Domain lookup(const Symbol &key) const;
  Domain lookup(const Value &key) const;
  void insert(const Symbol &key, const Domain &val);
  void insert(const Register &key, const Domain &val);
};

class Env {
  std::unordered_set<Map> heap_, stack_;

 public:
  Env() = default;
  explicit Env(const EnvSlice &slice);
  void merge(const EnvSlice &slice);
  void merge(const Env &env);
  EnvSlice concat() const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA
