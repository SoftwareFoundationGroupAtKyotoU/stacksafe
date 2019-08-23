#include "value.hpp"
#include <llvm/IR/Value.h>

namespace stacksafe {
namespace value {
Value::Value(const llvm::Value& v) : val_{&v} {}
}  // namespace value
}  // namespace stacksafe
