#ifndef INCLUDE_GUARD_BA783AFD_E463_407E_B980_287A32202B17
#define INCLUDE_GUARD_BA783AFD_E463_407E_B980_287A32202B17

#include <optional>
#include <string>
#include "json_fwd.hpp"
#include "type.hpp"

namespace llvm {
class Value;
}

namespace stacksafe {

class Value {
  std::optional<int> num_;
  Type type_;
  std::string repr_;

 public:
  explicit Value(const llvm::Value& v);
  std::optional<int> number() const;
  const Type& type() const;
  std::string repr() const;
  bool is_register() const;
};
bool operator<(const Value& lhs, const Value& rhs);
void to_json(Json& j, const Value& x);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_BA783AFD_E463_407E_B980_287A32202B17