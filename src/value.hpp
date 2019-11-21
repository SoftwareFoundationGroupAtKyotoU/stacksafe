#ifndef INCLUDE_GUARD_339D16F3_498E_420D_A302_BE9C21A36707
#define INCLUDE_GUARD_339D16F3_498E_420D_A302_BE9C21A36707

#include "base.hpp"

namespace llvm {
class AllocaInst;
class Argument;
class Instruction;
class hash_code;
}  // namespace llvm

namespace stacksafe {

class Value {
  Base key_;
  explicit Value(const llvm::Value &val);
  Value(const void *sym, bool is_arg);

 public:
  const llvm::Value *value() const;
  bool is_global() const;
  bool is_local() const;
  bool is_argument() const;
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
