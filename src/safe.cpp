#include "safe.hpp"

namespace stacksafe {

Safe::Safe() : safe_{false} {}
Safe::operator bool() const {
  return safe_;
}
void Safe::unsafe() {
  safe_ = false;
}

}  // namespace stacksafe
