#include "interpret.hpp"

namespace stacksafe {
  Interpret::Interpret(Env &env)
    : env_(env) {
  }
  bool Interpret::visit(llvm::Instruction &I) {
    llvm::outs() << I << endl;
    if (!Base::visit(I)) {
      llvm::outs() << env_;
      return false;
    }
    return true;
  }
  bool Interpret::visitInstruction(llvm::Instruction &I) {
    ClassNameVisitor name;
    llvm::outs() << name.visit(I) << endl;
    return false;
  }
  bool Interpret::visitAllocaInst(llvm::AllocaInst &I) {
    if (auto reg = make_register(I)) {
      return env_.alloca(*reg);
    }
    return error(__func__);
  }
  bool Interpret::error(const char *name) {
    llvm::errs() << "Error: unknown error in " << name << endl;
    return false;
  }
}
