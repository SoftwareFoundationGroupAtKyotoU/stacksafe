#ifndef INCLUDE_GUARD_649A01D6_F0F8_4589_8C81_4B1FCA2FD120
#define INCLUDE_GUARD_649A01D6_F0F8_4589_8C81_4B1FCA2FD120

#include <cstddef>

namespace llvm {
class Value;
}

namespace stacksafe {

class Value {
  const llvm::Value* const val_;

 public:
  enum Kind { REGISTER, GLOBAL, CONSTANT, OTHER };
  explicit Value(const llvm::Value* v);
  const llvm::Value* value() const;
  std::size_t hash() const;
  Kind kind() const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_649A01D6_F0F8_4589_8C81_4B1FCA2FD120
