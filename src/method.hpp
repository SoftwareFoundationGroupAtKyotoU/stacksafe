#ifndef INCLUDE_GUARD_6D412E24_64BA_4C27_BEA5_DB1CAFCC48BD
#define INCLUDE_GUARD_6D412E24_64BA_4C27_BEA5_DB1CAFCC48BD

#include "environment.hpp"
#include <llvm/IR/InstVisitor.h>

namespace stacksafe {
  class Method : public llvm::InstVisitor<Method, bool> {
    using Base = llvm::InstVisitor<Method, bool>;
    Env &env_;
  public:
    explicit Method(Env &env);
    bool visit(llvm::Instruction &I);
  };
}

#endif//INCLUDE_GUARD_6D412E24_64BA_4C27_BEA5_DB1CAFCC48BD
