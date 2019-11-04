#include "number.hpp"

namespace stacksafe {

Number::Number(int n) : num_{n} {}
int Number::number() const {
  return num_;
}

}  // namespace stacksafe
