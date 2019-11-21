#include "key.hpp"
#include <cassert>

namespace stacksafe {

const Key::Base Key::symbol_flag{0x1};
const Key::Base Key::global_flag{0x2};
Key::Key() : sym_{symbol_flag | global_flag} {}
Key::Key(const llvm::Value& val) : val_{&val} {}
Key::Key(const llvm::Value& val, bool is_arg) : Key{val} {
  static_assert(sizeof(val_) == sizeof(sym_));
  sym_ |= symbol_flag;
  if (is_arg) {
    sym_ |= global_flag;
  }
}
bool Key::is_symbol() const {
  return sym_ & symbol_flag;
}
bool Key::is_register() const {
  return !is_symbol();
}
bool Key::is_local() const {
  assert(is_symbol());
  return (sym_ & global_flag) == 0;
}
bool Key::is_global() const {
  assert(is_symbol());
  return sym_ == (symbol_flag | global_flag);
}
bool Key::is_argument() const {
  assert(is_symbol());
  return !is_global() && !is_local();
}

}  // namespace stacksafe
