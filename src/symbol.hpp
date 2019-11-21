#ifndef INCLUDE_GUARD_339D16F3_498E_420D_A302_BE9C21A36707
#define INCLUDE_GUARD_339D16F3_498E_420D_A302_BE9C21A36707

#include "key.hpp"

namespace llvm {
class AllocaInst;
class Argument;
class hash_code;
}  // namespace llvm

namespace stacksafe {

class Symbol {
  Key key_;
  explicit Symbol(const llvm::Value &val);
  Symbol(const void *sym, bool is_arg);

 public:
  const llvm::Value *value() const;
  bool is_global() const;
  bool is_local() const;
  bool is_argument() const;
  static Symbol get_symbol();
  static Symbol get_local(const llvm::AllocaInst &v);
  static Symbol get_arg(const llvm::Argument &v);
  static Symbol get_register(const llvm::Value &v);
  bool operator==(const Symbol &sym) const;
  bool operator<(const Symbol &sym) const;
  friend llvm::hash_code hash_value(const Symbol &sym);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_339D16F3_498E_420D_A302_BE9C21A36707
