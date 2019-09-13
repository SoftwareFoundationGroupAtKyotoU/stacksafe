#ifndef INCLUDE_GUARD_40DF6A39_0547_47DA_AC0B_47A60CCCE5DE
#define INCLUDE_GUARD_40DF6A39_0547_47DA_AC0B_47A60CCCE5DE

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

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_40DF6A39_0547_47DA_AC0B_47A60CCCE5DE
