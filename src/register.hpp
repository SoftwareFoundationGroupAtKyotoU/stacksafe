#ifndef INCLUDE_GUARD_8F170DBC_839A_403D_A59A_EA8DF99056AA
#define INCLUDE_GUARD_8F170DBC_839A_403D_A59A_EA8DF99056AA

#include "value.hpp"

#define IMPLICIT  // explicitly mark as implicit conversion constructor

namespace llvm {
class Argument;
class Instruction;
}  // namespace llvm

namespace stacksafe {

class Register : private Value {
 public:
  using Value::Kind, Value::value, Value::kind;
  IMPLICIT Register(const llvm::Argument& v);
  IMPLICIT Register(const llvm::Instruction& v);
  IMPLICIT Register(const llvm::Value& v);
  IMPLICIT Register(const Value& v);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_8F170DBC_839A_403D_A59A_EA8DF99056AA
