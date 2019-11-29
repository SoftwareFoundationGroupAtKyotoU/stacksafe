#ifndef INCLUDE_GUARD_339D16F3_498E_420D_A302_BE9C21A36707
#define INCLUDE_GUARD_339D16F3_498E_420D_A302_BE9C21A36707

#include <tuple>
#include "register.hpp"
#include "symbol.hpp"

namespace llvm {
class AllocaInst;
class Argument;
class Instruction;
class Value;
class hash_code;
}  // namespace llvm

namespace stacksafe {

class Value {
  union {
    Symbol sym_;
    Register reg_;
    const void *ptr_;
  };
  explicit Value(const Symbol &sym);
  explicit Value(const Register &sym);

 public:
  const llvm::Value *value() const;
  const void *ptr() const;
  bool is_symbol() const;
  const Symbol *as_symbol() const;
  const Register *as_register() const;
  static Value get_symbol();
  static Value get_symbol(const llvm::AllocaInst &v);
  static Value get_symbol(const llvm::Argument &v);
  static Value get_register(const llvm::Argument &v);
  static Value get_register(const llvm::Instruction &v);
};
bool operator==(const Value &lhs, const Value &rhs);
bool operator<(const Value &lhs, const Value &rhs);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_339D16F3_498E_420D_A302_BE9C21A36707
