#ifndef INCLUDE_GUARD_A3B67C9B_CF18_446A_AC08_280B66DC795B
#define INCLUDE_GUARD_A3B67C9B_CF18_446A_AC08_280B66DC795B

#include <string>
#include "number.hpp"

namespace stacksafe {
class Register;

class Symbol : private Number {
  static const Symbol global_;
  using Super = Number;
  explicit Symbol(const Number &n);

 public:
  explicit Symbol(int n);
  const Number &number() const;
  bool is_local() const;
  static Symbol global();
  static Symbol local(const Register &reg);
};
bool operator<(const Symbol &lhs, const Symbol &rhs);
std::string to_str(const Symbol &symbol);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_A3B67C9B_CF18_446A_AC08_280B66DC795B
