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
    llvm::outs() << "Analyzer: ";
    llvm::outs().write_escaped(f.getName()) << "\n";
    llvm::outs() << f;
    Log log{f};
    Abstract abstract{log};
    abstract.interpret(f);
    Json j = abstract;
    llvm::outs() << j.dump(2) << "\n";
    log.print();
    return false;
  }
};

char Analyzer::ID = 0;
static llvm::RegisterPass<Analyzer> registerpass("stacksafe",
                                                 "stacksafe analysis pass",
                                                 false, true);
}  // namespace stacksafe
