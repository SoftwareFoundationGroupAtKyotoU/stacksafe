#ifndef INCLUDE_GUARD_339D16F3_498E_420D_A302_BE9C21A36707
#define INCLUDE_GUARD_339D16F3_498E_420D_A302_BE9C21A36707

#include <string>
#include "number.hpp"
#include "symbol.hpp"

namespace stacksafe {

class Register : public Symbol {
  const int num_;
  friend class Cache;
  explicit Register(int n);

 public:
  using Symbol::number;
  bool is_local() const;
  static const Register &get_global();
};
bool operator<(const Register &lhs, const Register &rhs);
std::string to_str(const Register &reg);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_339D16F3_498E_420D_A302_BE9C21A36707
