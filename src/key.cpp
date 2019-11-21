#include "key.hpp"
#include <cassert>

namespace stacksafe {

Key::Key() : sym_{3} {}
Key::Key(const llvm::Value& val) : val_{&val} {}
Key::Key(const llvm::Value& val, bool is_arg) : Key{val} {
  static_assert(sizeof(val_) == sizeof(sym_));
  sym_ |= 1;
  if (is_arg) {
    sym_ |= 2;
  }
}
bool Key::is_symbol() const {
  return sym_ & 1;
}
bool Key::is_register() const {
  return !is_symbol();
}
bool Key::is_local() const {
  assert(is_symbol());
  return (sym_ & 2) == 0;
}
bool Key::is_global() const {
  assert(is_symbol());
  return sym_ == 3;
}
bool Key::is_argument() const {
  assert(is_symbol());
  return !is_global() && !is_local();
}

}  // namespace stacksafe
