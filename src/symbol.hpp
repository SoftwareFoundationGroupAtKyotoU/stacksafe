#ifndef INCLUDE_GUARD_339D16F3_498E_420D_A302_BE9C21A36707
#define INCLUDE_GUARD_339D16F3_498E_420D_A302_BE9C21A36707

#include <functional>
#include "value.hpp"

namespace llvm {
class AllocaInst;
class Argument;
}  // namespace llvm

namespace stacksafe {

class Symbol : private Value {
  explicit Symbol(const Value &v);

 public:
  const Value &value() const;
  bool is_global() const;
  bool is_local() const;
  bool is_arg() const;
  static const Symbol &get_global();
  static Symbol get_local(const llvm::AllocaInst &v);
  static Symbol get_arg(const llvm::Argument &v);
};
bool operator<(const Symbol &lhs, const Symbol &rhs);
bool operator==(const Symbol &lhs, const Symbol &rhs);

}  // namespace stacksafe

namespace std {
template <>
struct hash<stacksafe::Symbol> {
  size_t operator()(const stacksafe::Symbol &sym) const;
};
}  // namespace std

#endif  // INCLUDE_GUARD_339D16F3_498E_420D_A302_BE9C21A36707
