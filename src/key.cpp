#include "key.hpp"
#include <cassert>

namespace stacksafe {
namespace {
const std::uintptr_t symbol_flag{0x1};
const std::uintptr_t global_flag{0x3};
}  // namespace

Base::Base(const llvm::Value& val) : val_{&val} {}
Base::Base(Ptr val, bool is_arg)
    : sym_{(val ? reinterpret_cast<Int>(val) : global_flag) |
           (is_arg ? global_flag : symbol_flag)} {
  static_assert(sizeof(Ptr) == sizeof(Int));
  assert((reinterpret_cast<Int>(val) & global_flag) == 0);
}
const llvm::Value* Base::value() const {
  if (is_register()) {
    return static_cast<const llvm::Value*>(val_);
  } else if (is_global()) {
    return nullptr;
  } else {
    return reinterpret_cast<const llvm::Value*>(sym_ & ~global_flag);
  }
}
bool Base::is_symbol() const {
  return sym_ & symbol_flag;
}
bool Base::is_register() const {
  return !is_symbol();
}
bool Base::is_local() const {
  assert(is_symbol());
  return (sym_ & global_flag) == symbol_flag;
}
bool Base::is_global() const {
  assert(is_symbol());
  return sym_ == global_flag;
}
bool Base::is_argument() const {
  assert(is_symbol());
  return !is_global() && !is_local();
}
bool Base::equals(const Base& lhs, const Base& rhs) {
  return lhs.sym_ == rhs.sym_;
}
bool Base::less(const Base& lhs, const Base& rhs) {
  return lhs.sym_ < rhs.sym_;
}

}  // namespace stacksafe
