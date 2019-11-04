#include "symbol.hpp"
#include "register.hpp"

namespace stacksafe {

const Symbol Symbol::global_{-1};
const Number &Symbol::number() const {
  return *this;
}
Symbol Symbol::global() {
  return Symbol{-1};
}
Symbol Symbol::local(const Register &reg) {
  return Symbol{reg.number().value()};
}
bool Symbol::is_global() const {
  return number().value() < 0;
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
    return prefix + to_str(symbol.number());
  }
}

}  // namespace stacksafe
