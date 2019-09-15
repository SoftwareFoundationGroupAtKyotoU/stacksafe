#ifndef INCLUDE_GUARD_5A10BD0B_E34B_4331_BBA6_9475637B0566
#define INCLUDE_GUARD_5A10BD0B_E34B_4331_BBA6_9475637B0566

#include <string>
#include "json_fwd.hpp"
#include "token.hpp"

namespace llvm {
class Value;
}

namespace stacksafe {
class Fabric;

class Value : private Token {
 public:
  enum class Kind { REGISTER, CONSTANT, GLOBAL };

 private:
  static const std::string prefix_;
  const llvm::Value *const value_;
  Kind kind_;
  Value(int n, const llvm::Value &v);
  Value(const llvm::Value &v, Kind k);

 public:
  using Token::number, Token::type;
  static Value make(const llvm::Value &v);
  explicit Value(int n);
  const llvm::Value *get() const;
  std::string repr() const;
  int kind() const;
  bool is_register() const;
};
bool operator<(const Value &lhs, const Value &rhs);
void to_json(Json &j, const Value &x);
Fabric dump(const Value &value);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_5A10BD0B_E34B_4331_BBA6_9475637B0566
