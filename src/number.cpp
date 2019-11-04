#include "number.hpp"

namespace stacksafe {

Number::Number(int n) : num_{n} {}
int Number::number() const {
  return num_;
}
bool operator<(const Number& lhs, const Number& rhs) {
  return lhs.number() < rhs.number();
}

}  // namespace stacksafe
