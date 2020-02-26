#ifndef INCLUDE_GUARD_EB0B0AEA_791C_48A0_BE6F_8E63AC0A1D93
#define INCLUDE_GUARD_EB0B0AEA_791C_48A0_BE6F_8E63AC0A1D93

#include <map>
#include <optional>
#include <string>
#include <string_view>
#include "matrix.hpp"

namespace stacksafe {
class EffectLine;

class Effect {
  Matrix matrix_;
  std::string name_;
  explicit Effect(const EffectLine& line);

 public:
  static std::optional<Effect> make(std::string_view v);
  const std::string& name() const;
  const Arity& arity() const;
  bool depends(Index from, Index to) const;
};

class EffectMap : private std::map<std::string, Effect> {
  using Super = std::map<std::string, Effect>;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_EB0B0AEA_791C_48A0_BE6F_8E63AC0A1D93
