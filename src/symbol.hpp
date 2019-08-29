#ifndef INCLUDE_GUARD_5A26F608_5BA4_4080_8212_72E9DC406772
#define INCLUDE_GUARD_5A26F608_5BA4_4080_8212_72E9DC406772

#include <string>
#include "json_fwd.hpp"
#include "type.hpp"

namespace llvm {
class Type;
}

namespace stacksafe {

class Symbol {
  static int current_;
  int num_;
  Type type_;
  Symbol(int n, const llvm::Type *t);

 public:
  static Symbol create(const llvm::Type *t = nullptr);
  int get_num() const;
  const Type &get_type() const;
};
bool operator<(const Symbol &lhs, const Symbol &rhs);
void to_json(Json &j, const Symbol &x);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_5A26F608_5BA4_4080_8212_72E9DC406772
