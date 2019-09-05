#include "symbol.hpp"
#include <string>
#include "json.hpp"

namespace stacksafe {

const std::string Symbol::prefix_{"#"};
int Symbol::current_ = 0;
Symbol::Symbol(int n, const Type &t) : num_{n}, type_{t} {}
Symbol Symbol::global(const Type &t) { return Symbol{-1, t}; }
Symbol Symbol::create(const Type &t) { return Symbol{current_++, t}; }
void Symbol::reset() { current_ = 0; }
int Symbol::number() const { return num_; }
const Type &Symbol::type() const { return type_; }
std::string Symbol::repr(std::size_t width) const {
  std::string ret;
  if (is_global()) {
    ret.append("g");
  } else {
    ret.insert(0, length() < width ? width - length() : 0, '0');
    ret.append(std::to_string(num_));
  }
  ret.append(type_.repr());
  ret.insert(0, prefix_);
  return ret;
}
std::size_t Symbol::length() const { return std::to_string(num_).size(); }
bool Symbol::is_global() const { return num_ < 0; }
bool operator<(const Symbol &lhs, const Symbol &rhs) {
  return lhs.number() < rhs.number();
}
void to_json(Json &j, const Symbol &x) { j = x.repr(); }

}  // namespace stacksafe
