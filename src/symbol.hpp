#ifndef INCLUDE_GUARD_A3B67C9B_CF18_446A_AC08_280B66DC795B
#define INCLUDE_GUARD_A3B67C9B_CF18_446A_AC08_280B66DC795B

#include <string>
#include "json_fwd.hpp"
#include "token.hpp"

namespace stacksafe {
class Fabric;

class Symbol : private Token {
  static const std::string prefix_;
  static constexpr int current_init = 0;
  static int current_;
  enum class Kind { STATIC, OUTLIVING, AUTO } kind_;
  Symbol(Kind k, int n, const Type &t);

 public:
  using Token::number, Token::type;
  static Symbol global();
  static Symbol make(const Type &t);
  static void reset();
  int kind() const;
  std::string repr() const;
};
bool operator<(const Symbol &lhs, const Symbol &rhs);
void to_json(Json &j, const Symbol &x);
Fabric dump(const Symbol &symbol);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_A3B67C9B_CF18_446A_AC08_280B66DC795B
