#include "interpret.hpp"

namespace stacksafe {
  Interpret::Interpret(Env &env)
    : env_(env) {
  }
  bool Interpret::visit(llvm::Instruction &I) {
    auto ret = Base::visit(I);
    llvm::outs() << I << endl;
    llvm::outs() << env_;
    return ret;
  }
  bool Interpret::visitInstruction(llvm::Instruction &I) {
    ClassNameVisitor name;
    llvm::outs() << name.visit(I) << endl;
    return true;
  }
  bool Interpret::visitAllocaInst(llvm::AllocaInst &I) {
    if (auto reg = make_register(I)) {
      return env_.alloca(*reg);
    }
    error(__func__);
    return false;
  }
  void Interpret::error(const char *name) {
    llvm::errs() << "Error: unknown error in " << name << endl;
  }
}
