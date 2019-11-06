#include "value.hpp"
#include <llvm/IR/Instructions.h>
#include <functional>

namespace stacksafe {
namespace {
bool is_global(const llvm::Constant* c) {
  if (llvm::isa<llvm::GlobalValue>(c)) {
    return true;
  }
  for (unsigned i = 0; i < c->getNumOperands(); ++i) {
    assert(llvm::isa<llvm::Constant>(c->getOperand(i)) &&
           "constant has non-constant");
    if (auto v = llvm::dyn_cast<llvm::Constant>(c->getOperand(i));
        is_global(v)) {
      return true;
    }
  }
  return false;
}
}  // namespace

Value::Value(const llvm::Value* v) : val_{v} {}
const llvm::Value* Value::value() const {
  return val_;
}
std::size_t Value::hash() const {
  return std::hash<const llvm::Value*>{}(val_);
}

}  // namespace stacksafe
