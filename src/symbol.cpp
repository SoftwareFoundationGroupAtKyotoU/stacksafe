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
bool Symbol::is_local() const {
  return kind_ == Kind::AUTO;
}
bool operator<(const Symbol &lhs, const Symbol &rhs) {
  return lhs.number() < rhs.number();
}
std::string to_str(const Symbol &symbol) {
  static const std::string prefix{"&"};
  static const std::string global{"@"};
  if (symbol.number() < 0) {
    return prefix + global;
  } else {
    return prefix + std::to_string(symbol.number());
  }
}
Fabric dump(const Symbol &symbol) {
  Fabric ret;
  return ret.append(to_str(symbol)).quote();
}

}  // namespace stacksafe
