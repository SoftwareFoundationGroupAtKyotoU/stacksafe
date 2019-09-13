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
Symbol::Symbol(Kind k, int n, const Type &t) : Token{n, t}, kind_{k} {}
Symbol Symbol::global() { return Symbol{STATIC, current_init, Type{nullptr}}; }
Symbol Symbol::make(const Type &t) { return Symbol{AUTO, ++current_, t}; }
void Symbol::reset() { current_ = current_init; }
auto Symbol::kind() const -> Kind { return kind_; }
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
void to_json(Json &j, const Symbol &x) { j = x.repr() + x.type().repr(); }
Fabric dump(const Symbol &symbol) {
  Fabric ret;
  return ret.append(symbol.repr()).quote();
}

}  // namespace stacksafe
