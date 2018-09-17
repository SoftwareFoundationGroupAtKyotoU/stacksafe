#include "analyzer.hpp"

namespace stacksafe {
  char Analyzer::ID = 0;
  Analyzer::Analyzer()
    : llvm::FunctionPass(ID)
  {}
  bool Analyzer::runOnFunction(llvm::Function &F) {
    return false;
  }
}
