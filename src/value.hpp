#ifndef INCLUDE_GUARD_339D16F3_498E_420D_A302_BE9C21A36707
#define INCLUDE_GUARD_339D16F3_498E_420D_A302_BE9C21A36707

#include <tuple>
#include "base.hpp"
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

class Value : private Base {
  union {
    Symbol sym_;
    Register reg_;
    const void *ptr_;
  };
  explicit Value(const Symbol &sym);
  explicit Value(const Register &sym);
  explicit Value(const Base &base);
  explicit Value(const llvm::Value &val);
  Value(const void *sym, bool is_local);

 public:
  const llvm::Value *value() const;
  bool is_symbol() const;
  const Symbol *as_symbol() const;
  static Value get_symbol();
  static Value get_symbol(const llvm::AllocaInst &v);
  static Value get_symbol(const llvm::Argument &v);
  static Value get_register(const llvm::Argument &v);
  static Value get_register(const llvm::Instruction &v);
  bool operator==(const Value &sym) const;
  bool operator<(const Value &sym) const;
  friend llvm::hash_code hash_value(const Value &sym);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_339D16F3_498E_420D_A302_BE9C21A36707
