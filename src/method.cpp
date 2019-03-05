#include "method.hpp"

namespace stacksafe {
  Method::Method(Env &env)
    : env_(env) {
  }
  bool Method::visit(llvm::Instruction &I) {
    // auto ret = Base::visit(I);
    llvm::outs() << I << endl;
    llvm::outs() << env_;
    return false;
  }
}
