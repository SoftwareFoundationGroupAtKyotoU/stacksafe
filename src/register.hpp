#ifndef INCLUDE_GUARD_8F170DBC_839A_403D_A59A_EA8DF99056AA
#define INCLUDE_GUARD_8F170DBC_839A_403D_A59A_EA8DF99056AA

#include "value.hpp"

namespace llvm {
class Argument;
class Instruction;
}  // namespace llvm

namespace stacksafe {

class Register : private Value {
 public:
  using Value::value;
  Register(const llvm::Argument& v);
  Register(const llvm::Instruction& v);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_8F170DBC_839A_403D_A59A_EA8DF99056AA
