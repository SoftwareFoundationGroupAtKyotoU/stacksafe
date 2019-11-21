#include "key.hpp"

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

}  // namespace stacksafe
