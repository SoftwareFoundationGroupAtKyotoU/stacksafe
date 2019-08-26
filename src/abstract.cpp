#include "abstract.hpp"
#include "interpret.hpp"

void Abstraction::interpret(llvm::Function& f) {
  Interpret interpreter(*this);
  interpreter.visit(f);
}
void Abstraction::alloc(const Register& r) {}
