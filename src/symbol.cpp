#include "symbol.hpp"
#include "fabric.hpp"
#include "json.hpp"
#include "utility.hpp"

namespace stacksafe {

const std::string Symbol::prefix_{"#"};
int Symbol::current_ = Symbol::current_init;
Symbol::Symbol(Kind k, int n, const Type &t) : Token{n, t}, kind_{k} {}
Symbol Symbol::global() {
  return Symbol{Kind::STATIC, current_init, Type{nullptr}};
}
Symbol Symbol::make(const Type &t) { return Symbol{Kind::AUTO, ++current_, t}; }
void Symbol::reset() { current_ = current_init; }
std::string Symbol::repr() const {
  switch (kind_) {
  case Kind::AUTO:
    return prefix_ + to_alphabet(number());
  default:
    return prefix_ + "@";
  }
}
int Symbol::kind() const { return static_cast<int>(kind_); }
bool Symbol::is_local() const { return kind_ == Kind::AUTO; }
bool operator<(const Symbol &lhs, const Symbol &rhs) {
  if (lhs.is_local() && rhs.is_local()) {
    return lhs.number() < rhs.number();
  } else if (lhs.kind() == rhs.kind()) {
    return lhs.repr() < rhs.repr();
  } else {
    return lhs.kind() < rhs.kind();
  }
}
void to_json(Json &j, const Symbol &x) { j = x.repr() + x.type().repr(); }
Fabric dump(const Symbol &symbol) {
  Fabric ret;
  return ret.append(symbol.repr()).quote();
}

}  // namespace stacksafe
