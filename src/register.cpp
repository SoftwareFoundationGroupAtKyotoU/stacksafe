#include "register.hpp"
#include <llvm/IR/Instructions.h>

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

Register::Register(const llvm::Value& v) : Value{&v} {}

}  // namespace stacksafe
