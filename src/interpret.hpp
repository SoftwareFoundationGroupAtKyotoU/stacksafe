#ifndef INCLUDE_GUARD_6D412E24_64BA_4C27_BEA5_DB1CAFCC48BD
#define INCLUDE_GUARD_6D412E24_64BA_4C27_BEA5_DB1CAFCC48BD

#include "environment.hpp"
#include <llvm/IR/InstVisitor.h>

namespace stacksafe {
  class Interpret : public llvm::InstVisitor<Interpret, bool> {
    using Base = llvm::InstVisitor<Interpret, bool>;
    Env &env_;
  public:
    explicit Interpret(Env &env);
    bool visit(llvm::Instruction &I);
  };
}

#endif//INCLUDE_GUARD_6D412E24_64BA_4C27_BEA5_DB1CAFCC48BD
