#ifndef INCLUDE_GUARD_5A26F608_5BA4_4080_8212_72E9DC406772
#define INCLUDE_GUARD_5A26F608_5BA4_4080_8212_72E9DC406772

#include <string>
#include "json_fwd.hpp"
#include "token.hpp"

namespace stacksafe {

class Symbol : private Token {
  static const std::string prefix_;
  static int current_;
  using Token::Token;

 public:
  using Token::number, Token::type;
  static Symbol global(const Type &t);
  static Symbol create(const Type &t);
  static void reset();
  std::string repr() const;
  bool is_global() const;
};
bool operator<(const Symbol &lhs, const Symbol &rhs);
void to_json(Json &j, const Symbol &x);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_5A26F608_5BA4_4080_8212_72E9DC406772
