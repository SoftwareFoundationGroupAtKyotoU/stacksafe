#include "key.hpp"
#include <llvm/ADT/Hashing.h>
#include <cassert>

namespace stacksafe {

const Key::Base Key::symbol_flag{0x1};
const Key::Base Key::global_flag{0x3};
Key::Key(const llvm::Value& val) : val_{&val} {}
Key::Key(Ptr val, bool is_arg)
    : sym_{(val ? reinterpret_cast<Base>(val) : global_flag) |
           (is_arg ? global_flag : symbol_flag)} {
  static_assert(sizeof(Ptr) == sizeof(Base));
  assert((reinterpret_cast<Base>(val) & global_flag) == 0);
}
const llvm::Value* Key::value() const {
  if (is_register()) {
    return static_cast<const llvm::Value*>(val_);
  } else if (is_global()) {
    return nullptr;
  } else {
    return reinterpret_cast<const llvm::Value*>(sym_ & ~global_flag);
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
llvm::hash_code hash_value(const Key& key) {
  return llvm::hash_value(key.sym_);
}

}  // namespace stacksafe
