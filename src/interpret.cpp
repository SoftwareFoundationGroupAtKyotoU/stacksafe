#include "interpret.hpp"
#include "abstract.hpp"
#include "register.hpp"

Interpret::Interpret(Abstraction &a) : abst_{a} {}
void Interpret::visit(llvm::Function &f) { Base::visit(f); }
void Interpret::visit(llvm::BasicBlock &b) {
  Base::visit(b);
  abst_.show();
}
void Interpret::visit(llvm::Instruction &i) {
  llvm::errs() << i << "\n";
  Base::visit(i);
}
void Interpret::visitAllocaInst(llvm::AllocaInst &i) {
  Register r{i};
  abst_.alloc(r);
}
void Interpret::visitStoreInst(llvm::StoreInst &i) {
  auto src = i.getValueOperand();
  auto dst = i.getPointerOperand();
  if (dst && src) {
    abst_.store(Register{*src}, Register{*dst});
  }
}
