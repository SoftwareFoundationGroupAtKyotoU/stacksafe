#include "abstract.hpp"
#include "interpret.hpp"

void Abstraction::interpret(llvm::Function &f) {
  Interpret interpreter;
  interpreter.visit(f);
}
