#ifndef INCLUDE_GUARD_5A26F608_5BA4_4080_8212_72E9DC406772
#define INCLUDE_GUARD_5A26F608_5BA4_4080_8212_72E9DC406772

#include <string>
#include "json_fwd.hpp"
#include "type.hpp"

namespace stacksafe {

class Symbol {
  static const std::string prefix_;
  static int current_;
  int num_;
  Type type_;
  Symbol(int n, const Type &t);

 public:
  static Symbol global(const Type &t);
  static Symbol create(const Type &t);
  static void reset();
  int number() const;
  const Type &type() const;
  std::string repr() const;
  bool is_global() const;
};
bool operator<(const Symbol &lhs, const Symbol &rhs);
void to_json(Json &j, const Symbol &x);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_5A26F608_5BA4_4080_8212_72E9DC406772
