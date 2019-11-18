#ifndef INCLUDE_GUARD_8F170DBC_839A_403D_A59A_EA8DF99056AA
#define INCLUDE_GUARD_8F170DBC_839A_403D_A59A_EA8DF99056AA

#include "value.hpp"

namespace llvm {
class Argument;
class Instruction;
}  // namespace llvm

namespace stacksafe {

class Register : private Value {
  explicit Register(const Value& v);

 public:
  static Register make(const llvm::Instruction& i);
  const Value& value() const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_8F170DBC_839A_403D_A59A_EA8DF99056AA
