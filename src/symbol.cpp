#include "symbol.hpp"

namespace stacksafe {

int Symbol::current_ = 0;
Symbol::Symbol(int n, const llvm::Type *t) : num_{n}, type_{t} {}
Symbol Symbol::create(const llvm::Type *t) { return Symbol{current_++, t}; }
int Symbol::get_num() const { return num_; }
const llvm::Type *Symbol::get_type() const { return type_; }
bool Symbol::compare(const Symbol &that) const { return num_ < that.num_; }
std::string Symbol::to_str() const {
  using namespace std::string_literals;
  return "#"s + std::to_string(num_);
}
bool operator<(const Symbol &lhs, const Symbol &rhs) {
  return lhs.get_num() < rhs.get_num();
}
void to_json(Json &j, const Symbol &x) { j = x.to_str(); }

}  // namespace stacksafe
