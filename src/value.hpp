#ifndef INCLUDE_GUARD_649A01D6_F0F8_4589_8C81_4B1FCA2FD120
#define INCLUDE_GUARD_649A01D6_F0F8_4589_8C81_4B1FCA2FD120

namespace llvm {
class Value;
}

namespace stacksafe {

class Value {
  const llvm::Value* const val_;
  Value();

 public:
  const llvm::Value* value() const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_649A01D6_F0F8_4589_8C81_4B1FCA2FD120
