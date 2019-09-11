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
  bool runOnFunction(llvm::Function &f) override {
    log = Abstract::interpret(f);
    return false;
  }
  void print(llvm::raw_ostream &os, const llvm::Module *) const override {
    if (log) {
      log->print();
    } else {
      os << "Unknown error\n";
    }
  }
};

char Analyzer::ID = 0;
static llvm::RegisterPass<Analyzer> registerpass("stacksafe",
                                                 "stacksafe analysis pass",
                                                 false, true);
}  // namespace stacksafe
