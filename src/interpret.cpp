#include "interpret.hpp"

void Interpret::visit(llvm::Function &f) { Base::visit(f); }
void Interpret::visit(llvm::BasicBlock &b) { Base::visit(b); }
void Interpret::visit(llvm::Instruction &i) { llvm::errs() << i << "\n"; }
