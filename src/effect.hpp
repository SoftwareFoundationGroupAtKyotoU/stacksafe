#ifndef INCLUDE_GUARD_EB0B0AEA_791C_48A0_BE6F_8E63AC0A1D93
#define INCLUDE_GUARD_EB0B0AEA_791C_48A0_BE6F_8E63AC0A1D93

#include <map>
#include <optional>
#include <string>
#include <string_view>
#include "matrix.hpp"

namespace llvm {
class CallBase;
class raw_ostream;
}  // namespace llvm

namespace stacksafe {

class Effect {
  Matrix matrix_;
  std::string name_;

 public:
  Effect();
  const std::string &name() const;
  const Arity &arity() const;
  bool init(std::string_view v);
  bool depends(Index from, Index to) const;
  llvm::raw_ostream &print(llvm::raw_ostream &os) const;
};

class EffectMap : private std::map<std::string, Effect> {
  using Super = std::map<std::string, Effect>;

 public:
  Effect get(const llvm::CallBase &call) const;
  void load(const std::string &file);
  void save(const std::string &file) const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_EB0B0AEA_791C_48A0_BE6F_8E63AC0A1D93
