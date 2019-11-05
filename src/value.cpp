#include "value.hpp"

namespace stacksafe {

Value::Value(const llvm::Value* v) : val_{v} {}
const llvm::Value* Value::value() const {
  return val_;
}

}  // namespace stacksafe
