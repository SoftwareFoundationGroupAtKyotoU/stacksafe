#ifndef INCLUDE_GUARD_40DF6A39_0547_47DA_AC0B_47A60CCCE5DE
#define INCLUDE_GUARD_40DF6A39_0547_47DA_AC0B_47A60CCCE5DE

#include <string>
#include "json_fwd.hpp"
#include "type.hpp"

namespace stacksafe {

class Token {
  int num_;
  Type type_;

 protected:
  Token(int n, const Type &t);
  int number() const;
  const Type &type() const;
};

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

#endif  // INCLUDE_GUARD_40DF6A39_0547_47DA_AC0B_47A60CCCE5DE
