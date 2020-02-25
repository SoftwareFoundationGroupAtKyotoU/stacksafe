#ifndef INCLUDE_GUARD_EB0B0AEA_791C_48A0_BE6F_8E63AC0A1D93
#define INCLUDE_GUARD_EB0B0AEA_791C_48A0_BE6F_8E63AC0A1D93

#include <optional>
#include <string>
#include <string_view>
#include "matrix.hpp"

namespace stacksafe {
class EffectLine;

class Effect {
  Matrix mat_;
  std::string name_;
  std::size_t arity_;
  explicit Effect(const EffectLine& line);

 public:
  enum class Index : int {
    GLOBAL = -1,
    RETURN = -2,
    OTHERS = -3,
  };
  static std::optional<Effect> make(std::string_view v);

 private:
  std::size_t convert(Index index) const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_EB0B0AEA_791C_48A0_BE6F_8E63AC0A1D93
