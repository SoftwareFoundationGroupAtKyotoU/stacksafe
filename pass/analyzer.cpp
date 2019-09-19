#include <llvm/Pass.h>
#include <memory>
#include "abstract.hpp"
#include "json.hpp"

namespace stacksafe {

struct Analyzer : public llvm::FunctionPass {
  static char ID;
  std::unique_ptr<Abstract> abst;
  Analyzer() : llvm::FunctionPass{ID} {}
  bool runOnFunction(llvm::Function &f) override {
    if ((abst = std::make_unique<Abstract>(f))) {
      abst->interpret();
      abst->verify();
    }
    return false;
  }
  void print(llvm::raw_ostream &os, const llvm::Module *) const override {
    if (abst) {
      abst->print(os);
    }
    os.flush();
  }
};

char Analyzer::ID = 0;
static llvm::RegisterPass<Analyzer> registerpass("stacksafe",
                                                 "stacksafe analysis pass",
                                                 false, true);
}  // namespace stacksafe
