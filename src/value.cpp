#include "value.hpp"

namespace stacksafe {

Value::Value() : val_{nullptr} {}
const llvm::Value* Value::value() const {
  return val_;
}

}  // namespace stacksafe
