#include "interpret.hpp"
#include "abstract.hpp"
#include "register.hpp"

Interpret::Interpret(Abstraction &a) : abst_{a} {}
void Interpret::visit(llvm::Function &f) { Base::visit(f); }
void Interpret::visit(llvm::BasicBlock &b) { Base::visit(b); }
void Interpret::visit(llvm::Instruction &i) {
  llvm::errs() << i << "\n";
  Base::visit(i);
}
void Interpret::visitAllocaInst(llvm::AllocaInst &i) {
  Register r{i};
  abst_.alloc(r);
}
