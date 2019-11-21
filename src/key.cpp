#include "key.hpp"
#include <cassert>

namespace stacksafe {

const Key::Base Key::symbol_flag{0x1};
const Key::Base Key::global_flag{0x3};
Key::Key(Ptr ptr, Base flag) : sym_{reinterpret_cast<Base>(ptr) | flag} {}
Key::Key() : sym_{global_flag} {
  static_assert(sizeof(Ptr) == sizeof(Base));
}
Key::Key(const llvm::Value& val) : val_{&val} {}
Key::Key(const llvm::Value& val, bool is_arg)
    : Key{&val, is_arg ? global_flag : symbol_flag} {}
const llvm::Value* Key::value() const {
  return is_symbol() ? nullptr : reinterpret_cast<const llvm::Value*>(val_);
}
bool Key::is_symbol() const {
  return sym_ & symbol_flag;
}
bool Key::is_register() const {
  return !is_symbol();
}
bool Key::is_local() const {
  assert(is_symbol());
  return (sym_ & global_flag) == symbol_flag;
}
bool Key::is_global() const {
  assert(is_symbol());
  return sym_ == global_flag;
}
bool Key::is_argument() const {
  assert(is_symbol());
  return !is_global() && !is_local();
}
bool Key::equals(const Key& lhs, const Key& rhs) {
  return lhs.sym_ == rhs.sym_;
}
bool Key::less(const Key& lhs, const Key& rhs) {
  return lhs.sym_ < rhs.sym_;
}

}  // namespace stacksafe
