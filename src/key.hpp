#ifndef INCLUDE_GUARD_EE020F3B_04D6_40F2_943A_DF1081556287
#define INCLUDE_GUARD_EE020F3B_04D6_40F2_943A_DF1081556287

#include <cstdint>

namespace llvm {
class AllocaInst;
class Argument;
class Instruction;
class Value;
class hash_code;
}  // namespace llvm

namespace stacksafe {

class Key {
  using Ptr = const void*;
  using Base = std::uintptr_t;
  union {
    Ptr val_;
    Base sym_;
  };
  static const Base symbol_flag, global_flag;
  Key(Ptr ptr, Base flag);

 public:
  Key();
  explicit Key(const llvm::AllocaInst& val);
  explicit Key(const llvm::Argument& val);
  explicit Key(const llvm::Instruction& val);
  explicit Key(const llvm::Value& val);
  Key(const llvm::Value& val, bool is_arg);
  const llvm::Value* value() const;
  bool is_symbol() const;
  bool is_register() const;
  bool is_local() const;
  bool is_global() const;
  bool is_argument() const;
  static bool equals(const Key& lhs, const Key& rhs);
  static bool less(const Key& lhs, const Key& rhs);
  friend llvm::hash_code hash_value(const Key& key);
};
llvm::hash_code hash_value(const Key& key);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_EE020F3B_04D6_40F2_943A_DF1081556287
