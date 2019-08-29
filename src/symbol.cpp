#include "symbol.hpp"
#include <string>
#include "json.hpp"

namespace stacksafe {

int Symbol::current_ = 0;
Symbol::Symbol(int n, const Type &t) : num_{n}, type_{t} {}
Symbol Symbol::create(const Type &t) { return Symbol{current_++, t}; }
int Symbol::number() const { return num_; }
const Type &Symbol::type() const { return type_; }
bool operator<(const Symbol &lhs, const Symbol &rhs) {
  return lhs.number() < rhs.number();
}
void to_json(Json &j, const Symbol &x) {
  std::string str{"#"};
  str += std::to_string(x.number());
  str += x.type().repr();
  j = str;
}

}  // namespace stacksafe
