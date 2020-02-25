#ifndef INCLUDE_GUARD_EB0B0AEA_791C_48A0_BE6F_8E63AC0A1D93
#define INCLUDE_GUARD_EB0B0AEA_791C_48A0_BE6F_8E63AC0A1D93

#include <string>
#include "matrix.hpp"

namespace stacksafe {
class EffectLine;

class Effect {
  Matrix mat_;
  std::string name_;

 public:
  explicit Effect(const EffectLine& line);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_EB0B0AEA_791C_48A0_BE6F_8E63AC0A1D93
