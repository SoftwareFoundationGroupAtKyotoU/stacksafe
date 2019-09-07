#include "token.hpp"

namespace stacksafe {

Token::Token(int n, const Type& t) : num_{n}, type_{t} {}
int Token::number() const { return num_; }
const Type& Token::type() const { return type_; }

}  // namespace stacksafe
