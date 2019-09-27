#ifndef INCLUDE_GUARD_339D16F3_498E_420D_A302_BE9C21A36707
#define INCLUDE_GUARD_339D16F3_498E_420D_A302_BE9C21A36707

#include <string>

namespace stacksafe {
class Fabric;

class Register {
  static const std::string prefix;
  const int num_;

 public:
  explicit Register(int n);
  int number() const;
  std::string repr() const;
};
bool operator<(const Register& lhs, const Register& rhs);
Fabric dump(const Register& reg);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_339D16F3_498E_420D_A302_BE9C21A36707
