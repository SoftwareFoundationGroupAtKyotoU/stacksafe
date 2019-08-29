#include "symbol.hpp"
#include "io.hpp"
#include "json.hpp"

namespace stacksafe {

int Symbol::current_ = 0;
Symbol::Symbol(int n, const llvm::Type *t) : num_{n}, type_{t}, t_{t} {}
Symbol Symbol::create(const llvm::Type *t) { return Symbol{current_++, t}; }
int Symbol::get_num() const { return num_; }
const Type &Symbol::get_type() const { return t_; }
bool operator<(const Symbol &lhs, const Symbol &rhs) {
  return lhs.get_num() < rhs.get_num();
}
void to_json(Json &j, const Symbol &x) {
  std::string str{"#"};
  str += std::to_string(x.get_num());
  str += x.get_type().repr();
  j = str;
}

}  // namespace stacksafe
