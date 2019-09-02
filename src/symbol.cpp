#include "symbol.hpp"
#include <string>
#include "json.hpp"

namespace stacksafe {

int Symbol::current_ = 0;
Symbol::Symbol(int n, const Type &t) : num_{n}, type_{t} {}
Symbol Symbol::create(const Type &t) { return Symbol{current_++, t}; }
int Symbol::number() const { return num_; }
const Type &Symbol::type() const { return type_; }
std::string Symbol::repr() const {
  using namespace std::string_literals;
  return "#"s + std::to_string(num_) + type_.repr();
}
bool operator<(const Symbol &lhs, const Symbol &rhs) {
  return lhs.number() < rhs.number();
}
void to_json(Json &j, const Symbol &x) { j = x.repr(); }

}  // namespace stacksafe
