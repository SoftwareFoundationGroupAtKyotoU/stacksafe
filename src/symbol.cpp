#include "symbol.hpp"
#include "fabric.hpp"
#include "utility.hpp"

namespace stacksafe {

Symbol::Symbol(int n) : reg_{n} {}
Symbol Symbol::global() {
  return Symbol{-1};
}
Symbol Symbol::make(const Register &base) {
  return Symbol{base.number()};
}
int Symbol::number() const {
  return reg_.number();
}
bool Symbol::is_global() const {
  return number() < 0;
}
bool operator<(const Symbol &lhs, const Symbol &rhs) {
  return lhs.number() < rhs.number();
}
std::string to_str(const Symbol &symbol) {
  static const std::string prefix{"&"};
  static const std::string global{"@"};
  if (symbol.is_global()) {
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
