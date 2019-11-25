#ifndef INCLUDE_GUARD_339D16F3_498E_420D_A302_BE9C21A36707
#define INCLUDE_GUARD_339D16F3_498E_420D_A302_BE9C21A36707

#include <tuple>
#include "base.hpp"

namespace llvm {
class AllocaInst;
class Argument;
class Instruction;
class Value;
class hash_code;
}  // namespace llvm

namespace stacksafe {

class Value : private Base {
  explicit Value(const Base &base);
  explicit Value(const llvm::Value &val);
  Value(const void *sym, bool is_local);

 public:
  const llvm::Value *value() const;
  bool is_global() const;
  bool is_local() const;
  bool is_argument() const;
  static Value get_zero();
  static Value get_negative();
  static Value get_symbol();
  static Value get_symbol(const llvm::AllocaInst &v);
  static Value get_symbol(const llvm::Argument &v);
  static Value get_register(const llvm::Argument &v);
  static Value get_register(const llvm::Instruction &v);
  bool operator==(const Value &sym) const;
  bool operator<(const Value &sym) const;
  friend llvm::hash_code hash_value(const Value &sym);
};

class Pair : private std::tuple<Value, Value> {
  using Super = std::tuple<Value, Value>;

 public:
  Pair(const Value &key, const Value &val);
  const Value &key() const;
  const Value &val() const;
  static Pair get_zero();
  static Pair get_negative();
  bool operator==(const Pair &pair) const;
  bool operator<(const Pair &pair) const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_339D16F3_498E_420D_A302_BE9C21A36707
