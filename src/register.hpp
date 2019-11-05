#ifndef INCLUDE_GUARD_8F170DBC_839A_403D_A59A_EA8DF99056AA
#define INCLUDE_GUARD_8F170DBC_839A_403D_A59A_EA8DF99056AA

#include "value.hpp"

#define IMPLICIT  // explicitly mark as implicit conversion constructor

namespace llvm {
class Value;
}  // namespace llvm

namespace stacksafe {

class Register : private Value {
 public:
  enum Kind { REGISTER, GLOBAL, CONSTANT, OTHER };
  using Value::value;
  IMPLICIT Register(const llvm::Value& v);
  Kind kind() const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_8F170DBC_839A_403D_A59A_EA8DF99056AA
