#ifndef INCLUDE_GUARD_838FCB87_603F_4E6D_A973_29F6EB0FD779
#define INCLUDE_GUARD_838FCB87_603F_4E6D_A973_29F6EB0FD779

namespace llvm {
class Value;
}
namespace stacksafe {
namespace value {

class Value {
  const llvm::Value* const val_;

 public:
  explicit Value(const llvm::Value& v);
};

}  // namespace value
}  // namespace stacksafe

#endif  // INCLUDE_GUARD_838FCB87_603F_4E6D_A973_29F6EB0FD779
