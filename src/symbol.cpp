#include "symbol.hpp"
#include <string>
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

const std::string Symbol::prefix_{"#"};
int Symbol::current_ = current_init;
Symbol::Symbol(int n, const Type &t) : num_{n}, type_{t} {}
Symbol Symbol::global(const Type &t) { return Symbol{-1, t}; }
Symbol Symbol::create(const Type &t) { return Symbol{current_++, t}; }
void Symbol::reset() { current_ = current_init; }
int Symbol::number() const { return num_; }
const Type &Symbol::type() const { return type_; }
std::string Symbol::repr() const {
  if (is_global()) {
    return prefix_ + "*";
  } else {
    return prefix_ + to_ascii(num_);
  }
}
std::size_t Symbol::length() const { return std::to_string(num_).size(); }
bool Symbol::is_global() const { return num_ < current_init; }
bool operator<(const Symbol &lhs, const Symbol &rhs) {
  return lhs.number() < rhs.number();
}
void to_json(Json &j, const Symbol &x) { j = x.repr(); }

}  // namespace stacksafe
