#include "symbol.hpp"

int Symbol::current_ = 0;
Symbol::Symbol(int n) : num_{n} {}
Symbol Symbol::create() { return Symbol{current_++}; }
bool Symbol::compare(const Symbol &that) const { return num_ < that.num_; }
std::string Symbol::to_str() const {
  using namespace std::string_literals;
  return "#"s + std::to_string(num_);
}
