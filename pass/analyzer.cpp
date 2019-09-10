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
    log = std::make_unique<Log>(f);
    if (log) {
      Abstract abstract{f, *log};
      abstract.interpret(f);
    }
    return false;
  }
  void print(llvm::raw_ostream &os, const llvm::Module *) const override {
    if (log) {
      std::error_code error;
      auto name = log->get_filename();
      llvm::raw_fd_ostream file{name, error};
      llvm::errs() << "print to: " << name << "\n";
      if (error) {
        llvm::errs() << "Error: " << error.message() << "\n";
        log->print(llvm::outs());
      } else {
        log->print(file);
      }
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
