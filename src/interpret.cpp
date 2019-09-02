#include "interpret.hpp"
#include <llvm/Support/raw_ostream.h>
#include "json.hpp"
#include "value.hpp"

namespace stacksafe {

Interpret::Interpret(const Env &e) : env_{e} {}
const Env &Interpret::get() const { return env_; }
void Interpret::visit(llvm::Function &f) {
  for (auto &a : f.args()) {
    env_.argument(Value{a});
  }
  llvm::errs() << f;
}
void Interpret::visit(llvm::BasicBlock &b) {
  Base::visit(b);
  Json j = env_;
  llvm::errs() << j.dump(2) << "\n";
}
void Interpret::visit(llvm::Instruction &i) { Base::visit(i); }
void Interpret::visitAllocaInst(llvm::AllocaInst &i) { env_.alloc(Value{i}); }
void Interpret::visitStoreInst(llvm::StoreInst &i) {
  auto src = i.getValueOperand();
  auto dst = i.getPointerOperand();
  if (dst && src) {
    env_.store(Value{*src}, Value{*dst});
  }
}
void Interpret::visitLoadInst(llvm::LoadInst &i) {
  if (auto src = i.getPointerOperand()) {
    env_.load(Value{i}, Value{*src});
  }
}
void Interpret::visitBinaryOperator(llvm::BinaryOperator &i) {
  env_.binop(Value{i});
}

}  // namespace stacksafe
