#ifndef INCLUDE_GUARD_40DF6A39_0547_47DA_AC0B_47A60CCCE5DE
#define INCLUDE_GUARD_40DF6A39_0547_47DA_AC0B_47A60CCCE5DE

#include <string>
#include "json_fwd.hpp"
#include "type.hpp"

namespace llvm {
class Value;
}

namespace stacksafe {
class Fabric;

class Token {
  int num_;
  Type type_;

 protected:
  Token(int n, const Type &t);
  int number() const;
  const Type &type() const;
};

class Symbol : private Token {
  static const std::string prefix_;
  static constexpr int current_init = 0;
  static int current_;
  using Token::Token;

 public:
  using Token::number, Token::type;
  static Symbol global();
  static Symbol create(const Type &t);
  static void reset();
  std::string repr() const;
  bool is_global() const;
};
bool operator<(const Symbol &lhs, const Symbol &rhs);
void to_json(Json &j, const Symbol &x);
Fabric dump(const Symbol &symbol);

class Value : private Token {
  static const std::string prefix_;
  const llvm::Value *const value_;
  Value(int n, llvm::Type *t, const llvm::Value *v);

 public:
  using Token::number, Token::type;
  static Value create(const llvm::Value &v);
  explicit Value(int n);
  const llvm::Value *get() const;
  std::string repr() const;
  bool is_register() const;
};
bool operator<(const Value &lhs, const Value &rhs);
void to_json(Json &j, const Value &x);
Fabric dump(const Value &value);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_40DF6A39_0547_47DA_AC0B_47A60CCCE5DE
