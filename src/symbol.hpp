#ifndef INCLUDE_GUARD_A3B67C9B_CF18_446A_AC08_280B66DC795B
#define INCLUDE_GUARD_A3B67C9B_CF18_446A_AC08_280B66DC795B

#include <string>
#include "register.hpp"

namespace stacksafe {
class Fabric;

class Symbol {
  Register reg_;
  Symbol(int n);

 public:
  static Symbol global();
  static Symbol make(const Register &base);
  int number() const;
  bool is_global() const;
};
bool operator<(const Symbol &lhs, const Symbol &rhs);
std::string to_str(const Symbol &symbol);
Fabric dump(const Symbol &symbol);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_A3B67C9B_CF18_446A_AC08_280B66DC795B
