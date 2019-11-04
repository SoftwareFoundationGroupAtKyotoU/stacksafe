#include "symbol.hpp"
#include "register.hpp"

namespace stacksafe {

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
  return value() < 0;
}
bool operator<(const Symbol &lhs, const Symbol &rhs) {
  return lhs.value() < rhs.value();
}
std::string to_str(const Symbol &symbol) {
  static const std::string prefix{"&"};
  static const std::string global{"@"};
  if (symbol.is_global()) {
    return prefix + global;
  } else {
    return prefix + std::to_string(symbol.value());
  }
}

}  // namespace stacksafe
