#include "analyzer.hpp"
#include "visualize.hpp"
#include <llvm/IR/Function.h>
#include <llvm/Support/raw_ostream.h>

namespace stacksafe {
  char Analyzer::ID = 0;
  Analyzer::Analyzer()
    : llvm::FunctionPass(ID)
  {}
  void Analyzer::print(llvm::raw_ostream &O, const llvm::Module *M) const {
    llvm::outs()
      // << "Analyzer Pass for Stack Temporal Memory Safety"
      << endl;
  }
  bool Analyzer::runOnFunction(llvm::Function &F) {
    abst_ = std::make_unique<Abstraction>(F);
    abst_->proceed();
    return false;
  }
}

static llvm::RegisterPass<stacksafe::Analyzer> registerpass
("stacksafe", "StackSafe");
