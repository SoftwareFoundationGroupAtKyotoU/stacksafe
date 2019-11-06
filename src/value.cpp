#include "value.hpp"
#include <llvm/IR/Instructions.h>

namespace stacksafe {
namespace {
bool is_global(const llvm::Constant *c) {
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

Value::Value(const llvm::Value *v) : val_{v} {}
Value::operator bool() const {
  return val_ != nullptr;
}
const llvm::Value *Value::value() const {
  return val_;
}
std::size_t Value::hash() const {
  return std::hash<const llvm::Value *>{}(val_);
}
auto Value::kind() const -> Kind {
  const auto v = value();
  if (llvm::isa<llvm::Argument>(v)) {
    return Kind::REGISTER;
  } else if (auto c = llvm::dyn_cast<llvm::Constant>(v)) {
    return is_global(c) ? Kind::GLOBAL : Kind::CONSTANT;
  } else if (auto i = llvm::dyn_cast<llvm::Instruction>(v)) {
    if (i->isTerminator()) {
      return Kind::OTHER;
    } else if (llvm::isa<llvm::StoreInst>(i)) {
      return Kind::OTHER;
    } else if (llvm::isa<llvm::FenceInst>(i)) {
      return Kind::OTHER;
    } else if (auto c = llvm::dyn_cast<llvm::CallInst>(i);
               c && c->getFunctionType()->getReturnType()->isVoidTy()) {
      return Kind::OTHER;
    } else {
      return Kind::REGISTER;
    }
  } else {
    return Kind::OTHER;
  }
}
bool operator<(const Value &lhs, const Value &rhs) {
  return lhs.value() < rhs.value();
}
bool operator==(const Value &lhs, const Value &rhs) {
  return lhs.value() == rhs.value();
}

}  // namespace stacksafe

namespace std {
size_t hash<stacksafe::Value>::operator()(const stacksafe::Value &val) const {
  return val.hash();
}
}  // namespace std
