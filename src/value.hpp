#ifndef INCLUDE_GUARD_649A01D6_F0F8_4589_8C81_4B1FCA2FD120
#define INCLUDE_GUARD_649A01D6_F0F8_4589_8C81_4B1FCA2FD120

#include <functional>

#define IMPLICIT  // explicitly mark as implicit conversion constructor

namespace llvm {
class Value;
}

namespace stacksafe {

class Value {
  const llvm::Value *const val_;

 public:
  enum Kind { REGISTER, GLOBAL, CONSTANT, OTHER };
  Value();
  IMPLICIT Value(const llvm::Value *v);
  explicit operator bool() const;
  const llvm::Value *get() const;
  std::size_t hash() const;
  Kind kind() const;
};
bool operator<(const Value &lhs, const Value &rhs);
bool operator==(const Value &lhs, const Value &rhs);

}  // namespace stacksafe

namespace std {
template <>
struct hash<stacksafe::Value> {
  size_t operator()(const stacksafe::Value &val) const;
};
}  // namespace std

#endif  // INCLUDE_GUARD_649A01D6_F0F8_4589_8C81_4B1FCA2FD120
