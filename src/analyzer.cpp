#include <llvm/IR/Function.h>
#include <llvm/Pass.h>
#include <llvm/PassSupport.h>
#include <llvm/Support/raw_ostream.h>
#include "interpret.hpp"

struct Analyzer : public llvm::FunctionPass {
  static char ID;
  Analyzer() : llvm::FunctionPass{ID} {}
  virtual bool runOnFunction(llvm::Function &f) override {
    llvm::errs() << "Analyzer: ";
    llvm::errs().write_escaped(f.getName()) << "\n";
    Interpret interpreter;
    interpreter.visit(f);
    return false;
  }
};

char Analyzer::ID = 0;
static llvm::RegisterPass<Analyzer> registerpass("stacksafe",
                                                 "stacksafe analysis pass",
                                                 false, true);
