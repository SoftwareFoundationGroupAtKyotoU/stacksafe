#include "token.hpp"
#include "json.hpp"

namespace stacksafe {
namespace {
std::string to_ascii(int num) {
  std::string ret;
  for (; 0 < num; num /= 26) {
    ret.insert(0, 1, 'a' + --num % 26);
  }
  return ret;
}
constexpr int current_init = 0;
}  // namespace

Token::Token(int n, const Type &t) : num_{n}, type_{t} {}
int Token::number() const { return num_; }
const Type &Token::type() const { return type_; }

const std::string Symbol::prefix_{"#"};
int Symbol::current_ = current_init;
Symbol Symbol::global(const Type &t) { return Symbol{current_init, t}; }
Symbol Symbol::create(const Type &t) { return Symbol{++current_, t}; }
void Symbol::reset() { current_ = current_init; }
std::string Symbol::repr() const {
  if (is_global()) {
    return prefix_ + "*";
  } else {
    return prefix_ + to_ascii(number());
  }
}
bool Symbol::is_global() const { return number() <= current_init; }
bool operator<(const Symbol &lhs, const Symbol &rhs) {
  return lhs.number() < rhs.number();
}
void to_json(Json &j, const Symbol &x) { j = x.repr(); }

}  // namespace stacksafe
