#include <llvm/IR/Function.h>
#include <llvm/Pass.h>
#include <llvm/Support/raw_ostream.h>
#include <memory>
#include "abstract.hpp"
#include "debug.hpp"
#include "json.hpp"

namespace stacksafe {

struct Analyzer : public llvm::FunctionPass {
  static char ID;
  std::unique_ptr<Log> log;
  Analyzer() : llvm::FunctionPass{ID} {}
  virtual bool runOnFunction(llvm::Function &f) override {
    log = std::make_unique<Log>(f);
    if (log) {
      Abstract abstract{*log};
      abstract.interpret(f);
      log->print(llvm::errs());
    }
    return false;
  }
};

char Analyzer::ID = 0;
static llvm::RegisterPass<Analyzer> registerpass("stacksafe",
                                                 "stacksafe analysis pass",
                                                 false, true);
}  // namespace stacksafe
