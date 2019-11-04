#include "symbol.hpp"
#include "register.hpp"

namespace stacksafe {

const Symbol Symbol::global_{Number{-1}};
Symbol::Symbol(const Number &n) : Number{n} {}
Symbol::Symbol(int n) : Number{n} {}
const Number &Symbol::number() const {
  return *this;
}
bool Symbol::is_local() const {
  return global_ < *this;
}
Symbol Symbol::global() {
  return global_;
}
Symbol Symbol::local(const Register &reg) {
  return Symbol{reg.number()};
}
bool operator<(const Symbol &lhs, const Symbol &rhs) {
  return lhs.number() < rhs.number();
}
std::string to_str(const Symbol &symbol) {
  static const std::string prefix{"&"};
  static const std::string global{"@"};
  if (symbol.is_local()) {
    return prefix + to_str(symbol.number());
  } else {
    return prefix + global;
  }
}

}  // namespace stacksafe
