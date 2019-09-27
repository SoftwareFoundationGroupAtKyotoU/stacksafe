#ifndef INCLUDE_GUARD_A3B67C9B_CF18_446A_AC08_280B66DC795B
#define INCLUDE_GUARD_A3B67C9B_CF18_446A_AC08_280B66DC795B

#include <string>

namespace stacksafe {
class Fabric;

class Symbol {
  static const std::string prefix_;
  static constexpr int current_init = 0;
  static int current_;
  enum class Kind { STATIC, AUTO } kind_;
  const int num_;
  Symbol(Kind k, int n);

 public:
  static Symbol global();
  static Symbol make();
  static void reset();
  int number() const;
  std::string repr() const;
  bool is_local() const;
};
bool operator<(const Symbol &lhs, const Symbol &rhs);
Fabric dump(const Symbol &symbol);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_A3B67C9B_CF18_446A_AC08_280B66DC795B
