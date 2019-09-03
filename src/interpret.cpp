#include "interpret.hpp"
#include <vector>
#include "abstract.hpp"
#include "value.hpp"

namespace llvm {
class Use;
}

namespace stacksafe {

Interpret::Interpret(const Env &e, Todo &todo) : env_{e}, todo_{todo} {}
const Env &Interpret::get() const { return env_; }
void Interpret::visit(llvm::BasicBlock &b) { Base::visit(b); }
auto Interpret::visit(llvm::Instruction &i) -> RetTy { return Base::visit(i); }
auto Interpret::visitInstruction(llvm::Instruction &i) -> RetTy {
  if (!i.isTerminator()) {
    todo_.insert(&i);
  }
}
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
  env_.constant(Value{i});
}
auto Interpret::visitCmpInst(llvm::CmpInst &i) -> RetTy {
  env_.constant(Value{i});
}
auto Interpret::visitCallInst(llvm::CallInst &i) -> RetTy {
  std::vector<llvm::Use *> uses;
  for (auto &a : i.args()) {
    uses.push_back(&a);
  }
  env_.call(Value{i}, uses);
}

}  // namespace stacksafe
