#include "symbol.hpp"
#include "fabric.hpp"
#include "utility.hpp"

namespace stacksafe {

const std::string Symbol::prefix_{"#"};
int Symbol::current_ = Symbol::current_init;
Symbol::Symbol(Kind k, int n) : kind_{k}, num_{n}, reg_{n} {}
Symbol Symbol::global() {
  return Symbol{Kind::STATIC, current_init};
}
Symbol Symbol::make() {
  return Symbol{Kind::AUTO, ++current_};
}
Symbol Symbol::make(const Register &base) {
  return Symbol{Kind::AUTO, ++current_};
}
void Symbol::reset() {
  current_ = current_init;
}
int Symbol::number() const {
  return num_;
}
std::string Symbol::repr() const {
  switch (kind_) {
    default:
      [[fallthrough]];
    case Kind::STATIC:
      return prefix_ + "@";
    case Kind::AUTO:
      return prefix_ + to_alphabet(number());
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
