#include "symbol.hpp"
#include "fabric.hpp"
#include "utility.hpp"

namespace stacksafe {

const std::string Symbol::prefix_{"#"};
Symbol::Symbol(Kind k, int n) : kind_{k}, reg_{n} {}
Symbol Symbol::global() {
  return Symbol{Kind::STATIC, -1};
}
Symbol Symbol::make(const Register &base) {
  return Symbol{Kind::AUTO, base.number()};
}
int Symbol::number() const {
  return reg_.number();
}
std::string Symbol::repr() const {
  switch (kind_) {
    default:
      [[fallthrough]];
    case Kind::STATIC:
      return prefix_ + "@";
    case Kind::AUTO:
      return prefix_ + std::to_string(number());
  }
}
bool Symbol::is_local() const {
  return kind_ == Kind::AUTO;
}
bool operator<(const Symbol &lhs, const Symbol &rhs) {
  return lhs.number() < rhs.number();
}
Fabric dump(const Symbol &symbol) {
  Fabric ret;
  return ret.append(symbol.repr()).quote();
}

}  // namespace stacksafe
