#include "base.hpp"
#include <cassert>
#include <limits>

namespace stacksafe {
namespace {
const std::uintptr_t symbol_flag{0x1};
const std::uintptr_t global_flag{0x3};
}  // namespace

Base::Base(const llvm::Value& val) : reg_{&val} {}
Base::Base(Ptr val, bool is_local)
    : sym_{(val ? reinterpret_cast<Int>(val) : global_flag) |
           (is_local ? symbol_flag : global_flag)} {
  static_assert(sizeof(Ptr) == sizeof(Int));
  assert((reinterpret_cast<Int>(val) & global_flag) == 0);
}
const llvm::Value* Base::value() const {
  if (is_register()) {
    return static_cast<const llvm::Value*>(reg_);
  } else if (is_global()) {
    return nullptr;
  } else {
    return reinterpret_cast<const llvm::Value*>(sym_ & ~global_flag);
  }
}
auto Base::ptr() const -> Ptr {
  if (!is_symbol()) {
    return reg_;
  } else if (is_global()) {
    return nullptr;
  } else {
    return reinterpret_cast<Ptr>(sym_ & ~global_flag);
  }
}
auto Base::val() const -> Int {
  return sym_;
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
bool Base::equals(const Base& lhs, const Base& rhs) {
  return lhs.sym_ == rhs.sym_;
}
bool Base::less(const Base& lhs, const Base& rhs) {
  return lhs.sym_ < rhs.sym_;
}

}  // namespace stacksafe
