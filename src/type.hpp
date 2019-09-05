#ifndef INCLUDE_GUARD_DDB53FB4_0D81_45F7_AB62_9AD997BC6CFA
#define INCLUDE_GUARD_DDB53FB4_0D81_45F7_AB62_9AD997BC6CFA

#include <string>
#include "json_fwd.hpp"

namespace llvm {
class Type;
}

namespace stacksafe {

class Type {
  const llvm::Type* const type_;

 public:
  explicit Type(const llvm::Type* t);
  const llvm::Type* get() const;
  std::string repr() const;
  std::string repr_with_num(int num, std::size_t width) const;
  bool is_pointer() const;
  Type pointee_type() const;
};
void to_json(Json& j, const Type& x);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_DDB53FB4_0D81_45F7_AB62_9AD997BC6CFA
