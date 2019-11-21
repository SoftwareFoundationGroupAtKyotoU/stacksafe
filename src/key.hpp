#ifndef INCLUDE_GUARD_EE020F3B_04D6_40F2_943A_DF1081556287
#define INCLUDE_GUARD_EE020F3B_04D6_40F2_943A_DF1081556287

#include <cstdint>

namespace llvm {
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

 public:
  explicit Key(const llvm::Value& val);
  Key(Ptr val, bool is_arg);
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

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_EE020F3B_04D6_40F2_943A_DF1081556287
