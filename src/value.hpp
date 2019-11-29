#ifndef INCLUDE_GUARD_339D16F3_498E_420D_A302_BE9C21A36707
#define INCLUDE_GUARD_339D16F3_498E_420D_A302_BE9C21A36707

#include "register.hpp"
#include "symbol.hpp"

#define IMPLICIT /* explicitly mark as implicit */

namespace llvm {
class AllocaInst;
class Argument;
class Instruction;
class Value;
}  // namespace llvm

namespace stacksafe {

class Value {
  union {
    Symbol sym_;
    Register reg_;
    const void *ptr_;
  };

 public:
  IMPLICIT Value(const Symbol &sym);
  IMPLICIT Value(const Register &sym);
  const llvm::Value *value() const;
  const void *ptr() const;
  bool is_symbol() const;
  const Symbol *as_symbol() const;
  const Register *as_register() const;
};
bool operator==(const Value &lhs, const Value &rhs);
bool operator<(const Value &lhs, const Value &rhs);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_339D16F3_498E_420D_A302_BE9C21A36707
