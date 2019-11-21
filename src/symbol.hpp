#ifndef INCLUDE_GUARD_339D16F3_498E_420D_A302_BE9C21A36707
#define INCLUDE_GUARD_339D16F3_498E_420D_A302_BE9C21A36707

#include "key.hpp"

namespace llvm {
class Instruction;
class hash_code;
}  // namespace llvm

namespace stacksafe {
class Value;

class Symbol {
  Key key_;
  Symbol();
  explicit Symbol(const llvm::Instruction &val);
  Symbol(const void *sym, bool is_arg);

 public:
  Value value() const;
  bool is_global() const;
  bool is_local() const;
  bool is_argument() const;
  static Symbol get_global();
  static Symbol get_local(const llvm::AllocaInst &v);
  static Symbol get_arg(const llvm::Argument &v);
  static Symbol get_register(const llvm::Instruction &v);
  static std::size_t hash(const Symbol &sym);
  bool operator==(const Symbol &sym) const;
  bool operator<(const Symbol &sym) const;
  friend llvm::hash_code hash_value(const Symbol &sym);
};
llvm::hash_code hash_value(const Symbol &sym);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_339D16F3_498E_420D_A302_BE9C21A36707
