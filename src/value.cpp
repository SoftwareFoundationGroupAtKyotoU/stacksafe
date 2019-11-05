#include "value.hpp"
#include <functional>

namespace stacksafe {

Value::Value(const llvm::Value* v) : val_{v} {}
const llvm::Value* Value::value() const {
  return val_;
}
std::size_t Value::hash() const {
  return std::hash<const llvm::Value*>{}(val_);
}

}  // namespace stacksafe
