#include "error.hpp"

namespace stacksafe {

Error::Error() : flags_{0} {}
bool Error::is_error() const {
  return flags_ != 0;
}
void Error::error_return() {
  set(Kind::RETURN);
}
void Error::error_call() {
  set(Kind::CALL);
}
void Error::error_global() {
  set(Kind::GLOBAL);
}
void Error::error_argument() {
  set(Kind::ARGUMENT);
}
auto Error::shift(Kind k) -> Base {
  return 1 << static_cast<Base>(k);
}
void Error::set(Kind k) {
  flags_ |= shift(k);
}
bool Error::get(Kind k) const {
  return flags_ & shift(k);
}

}  // namespace stacksafe
