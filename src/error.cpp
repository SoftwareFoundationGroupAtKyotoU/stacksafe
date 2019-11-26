#include "error.hpp"
#include <llvm/Support/raw_ostream.h>

namespace stacksafe {

Error::Error() : flags_{0} {}
bool Error::is_error() const {
  return flags_ != 0;
}
void Error::error_return() {
  set(Kind::RETURN);
}
void Error::error_global() {
  set(Kind::GLOBAL);
}
void Error::error_argument() {
  set(Kind::ARGUMENT);
}
void Error::print(llvm::raw_ostream& os) const {
  static const char* const names[] = {"RETURN", "GLOBAL", "ARGUMENT"};
  static const auto kinds = {Kind::RETURN, Kind::GLOBAL, Kind::ARGUMENT};
  for (const auto k : kinds) {
    if (get(k)) {
      const auto i = static_cast<Base>(k);
      os << "ERROR: " << names[i] << "\n";
    }
  }
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
