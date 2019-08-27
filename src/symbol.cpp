#include "symbol.hpp"

namespace stacksafe {

int Symbol::current_ = 0;
Symbol::Symbol(int n) : num_{n} {}
Symbol Symbol::create() { return Symbol{current_++}; }
bool Symbol::compare(const Symbol &that) const { return num_ < that.num_; }
std::string Symbol::to_str() const {
  using namespace std::string_literals;
  return "#"s + std::to_string(num_);
}
bool operator<(const Symbol &lhs, const Symbol &rhs) {
  return lhs.compare(rhs);
}
void to_json(Json &j, const Symbol &x) { j = x.to_str(); }

}  // namespace stacksafe
