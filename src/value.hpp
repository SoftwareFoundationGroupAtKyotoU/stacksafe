#ifndef INCLUDE_GUARD_838FCB87_603F_4E6D_A973_29F6EB0FD779
#define INCLUDE_GUARD_838FCB87_603F_4E6D_A973_29F6EB0FD779

namespace llvm {
class Value;
}
namespace stacksafe {
namespace value {

class Value {
  const llvm::Value* const val_;
  int num_;

 public:
  explicit Value(const llvm::Value& v);
  int compare(const Value& that) const;
  bool is_register() const;
};
bool operator<(const Value& lhs, const Value& rhs);

}  // namespace value
}  // namespace stacksafe

#endif  // INCLUDE_GUARD_838FCB87_603F_4E6D_A973_29F6EB0FD779
