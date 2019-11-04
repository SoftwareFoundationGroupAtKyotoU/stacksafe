#ifndef INCLUDE_GUARD_339D16F3_498E_420D_A302_BE9C21A36707
#define INCLUDE_GUARD_339D16F3_498E_420D_A302_BE9C21A36707

#include <string>
#include "number.hpp"

namespace stacksafe {

class Register : private Number {
  using Super = Number;
  using Super::Super;

 public:
  using Super::value;
  const Number &number() const;
};
bool operator<(const Register &lhs, const Register &rhs);
std::string to_str(const Register &reg);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_339D16F3_498E_420D_A302_BE9C21A36707
