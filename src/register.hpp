#ifndef INCLUDE_GUARD_339D16F3_498E_420D_A302_BE9C21A36707
#define INCLUDE_GUARD_339D16F3_498E_420D_A302_BE9C21A36707

#include <string>
#include "number.hpp"

namespace stacksafe {

class Register : private Number {
  friend class Cache;
  explicit Register(int n);

 public:
  const Number &number() const;
  static const Register &get_global();
};
std::string to_str(const Register &reg);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_339D16F3_498E_420D_A302_BE9C21A36707
