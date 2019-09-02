#include "interpret.hpp"
#include "value.hpp"

namespace stacksafe {

Interpret::Interpret(const Env &e) : env_{e} {}
const Env &Interpret::get() const { return env_; }
void Interpret::visit(llvm::BasicBlock &b) { Base::visit(b); }
auto Interpret::visit(llvm::Instruction &i) -> RetTy { return Base::visit(i); }
auto Interpret::visitInstruction(llvm::Instruction &i) -> RetTy {}
auto Interpret::visitAllocaInst(llvm::AllocaInst &i) -> RetTy {
  env_.alloc(Value{i});
}
auto Interpret::visitStoreInst(llvm::StoreInst &i) -> RetTy {
  auto src = i.getValueOperand();
  auto dst = i.getPointerOperand();
  if (dst && src) {
    env_.store(Value{*src}, Value{*dst});
  }
}
auto Interpret::visitLoadInst(llvm::LoadInst &i) -> RetTy {
  if (auto src = i.getPointerOperand()) {
    env_.load(Value{i}, Value{*src});
  }
}
auto Interpret::visitBinaryOperator(llvm::BinaryOperator &i) -> RetTy {
  env_.binop(Value{i});
}

}  // namespace stacksafe
