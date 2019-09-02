#include <llvm/IR/Function.h>
#include <llvm/Pass.h>
#include <llvm/Support/raw_ostream.h>
#include "abstract.hpp"
#include "json.hpp"

namespace stacksafe {

struct Analyzer : public llvm::FunctionPass {
  static char ID;
  Analyzer() : llvm::FunctionPass{ID} {}
  virtual bool runOnFunction(llvm::Function &f) override {
    llvm::errs() << "Analyzer: ";
    llvm::errs().write_escaped(f.getName()) << "\n";
    llvm::errs() << f;
    Abstract abstract;
    abstract.interpret(f);
    Json j = abstract;
    llvm::errs() << j.dump(2) << "\n";
    return false;
  }
};

char Analyzer::ID = 0;
static llvm::RegisterPass<Analyzer> registerpass("stacksafe",
                                                 "stacksafe analysis pass",
                                                 false, true);
}  // namespace stacksafe
