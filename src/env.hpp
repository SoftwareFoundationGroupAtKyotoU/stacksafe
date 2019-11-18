#ifndef INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA
#define INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA

#include "map.hpp"

namespace llvm {
class Function;
}  // namespace llvm

namespace stacksafe {
class Register;

class EnvSlice {
  Map heap_, stack_;

 public:
  EnvSlice() = default;
  explicit EnvSlice(const llvm::Function &f);
  const Map &heap() const;
  const Map &stack() const;
  bool includes(const EnvSlice &that) const;
  void merge(const EnvSlice &that);
  Domain lookup(const Symbol &key) const;
  Domain lookup(const Value &key) const;
  void insert(const Symbol &key, const Domain &val);
  void insert(const Register &key, const Domain &val);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA
