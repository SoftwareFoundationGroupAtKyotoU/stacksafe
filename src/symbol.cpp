#include "symbol.hpp"
#include "fabric.hpp"
#include "json.hpp"

namespace stacksafe {
namespace {
std::string to_alphabet(int num) {
  std::string ret;
  for (; 0 < num; num /= 26) {
    ret.insert(0, 1, 'a' + --num % 26);
  }
  return ret;
}
}  // namespace

const std::string Symbol::prefix_{"#"};
int Symbol::current_ = Symbol::current_init;
Symbol Symbol::global() { return Symbol{current_init, Type{nullptr}}; }
Symbol Symbol::create(const Type &t) { return Symbol{++current_, t}; }
void Symbol::reset() { current_ = current_init; }
std::string Symbol::repr() const {
  if (is_local()) {
    return prefix_ + to_alphabet(number());
  } else {
    return prefix_ + "@";
  }
}
bool Symbol::is_local() const { return current_init < number(); }
bool operator<(const Symbol &lhs, const Symbol &rhs) {
  return lhs.number() < rhs.number();
}
void to_json(Json &j, const Symbol &x) { j = x.repr(); }
Fabric dump(const Symbol &symbol) {
  Fabric ret;
  return ret.append(symbol.repr()).append(dump(symbol.type())).quote();
}

}  // namespace stacksafe
