#include "interpret.hpp"

namespace stacksafe {
  Interpret::Interpret(Env &env)
    : env_(env) {
  }
  bool Interpret::visit(llvm::Instruction &I) {
    // auto ret = Base::visit(I);
    llvm::outs() << I << endl;
    llvm::outs() << env_;
    return false;
  }
}
