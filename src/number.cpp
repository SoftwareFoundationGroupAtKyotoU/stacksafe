#include "number.hpp"

namespace stacksafe {

Number::Number(int n) : num_{n} {}
int Number::value() const {
  return num_;
}
bool operator<(const Number& lhs, const Number& rhs) {
  return lhs.value() < rhs.value();
}
std::string to_str(const Number& number) {
  return std::to_string(number.value());
}

}  // namespace stacksafe
