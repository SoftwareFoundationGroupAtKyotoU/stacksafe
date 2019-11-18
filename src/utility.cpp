#include "utility.hpp"
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Value.h>
#include <llvm/Support/raw_ostream.h>
#include "value.hpp"

namespace stacksafe {

std::string to_str(const Value& v) {
  assert(v && "no operand at nullptr");
  std::string buf;
  llvm::raw_string_ostream stream{buf};
  v.get()->print(stream);
  return stream.str();
}
std::string get_operand(const Value& v) {
  assert(v && "no operand at nullptr");
  std::string buf;
  llvm::raw_string_ostream stream{buf};
  v.get()->printAsOperand(stream, false);
  return stream.str();
}
bool is_global(const llvm::Constant* c) {
  assert(c);
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
bool is_register(const llvm::Instruction* i) {
  assert(i);
  if (auto c = llvm::dyn_cast<llvm::CallInst>(i)) {
    return !c->getFunctionType()->getReturnType()->isVoidTy();
  } else if (i->isTerminator()) {
    return false;
  } else if (llvm::isa<llvm::StoreInst>(i)) {
    return false;
  } else if (llvm::isa<llvm::FenceInst>(i)) {
    return false;
  } else {
    return true;
  }
}

}  // namespace stacksafe
