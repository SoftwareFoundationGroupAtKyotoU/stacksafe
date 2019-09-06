#include <llvm/IR/Function.h>
#include <llvm/Pass.h>
#include <llvm/Support/raw_ostream.h>
#include "abstract.hpp"
#include "debug.hpp"
#include "json.hpp"

namespace stacksafe {

struct Analyzer : public llvm::FunctionPass {
  static char ID;
  Analyzer() : llvm::FunctionPass{ID} {}
  virtual bool runOnFunction(llvm::Function &f) override {
    Log log{f};
    Abstract abstract{log};
    abstract.interpret(f);
    log.print();
    Json j = abstract;
    llvm::outs() << j.dump(2) << "\n";
    return false;
  }
};

char Analyzer::ID = 0;
static llvm::RegisterPass<Analyzer> registerpass("stacksafe",
                                                 "stacksafe analysis pass",
                                                 false, true);
}  // namespace stacksafe
