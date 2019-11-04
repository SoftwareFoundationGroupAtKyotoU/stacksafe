#include <llvm/IR/Function.h>
#include <llvm/Pass.h>
#include <memory>
#include "abstract.hpp"
#include "json.hpp"
#include "utility.hpp"

namespace stacksafe {

struct Analyzer : public llvm::FunctionPass {
  static char ID;
  std::unique_ptr<Abstract> abst;
  std::string name;
  Analyzer() : llvm::FunctionPass{ID} {}
  bool runOnFunction(llvm::Function &f) override {
    llvm::outs().flush();
    llvm::errs().flush();
    name = f.getName().str();
    abst = std::make_unique<Abstract>(f);
    assert(abst && "allocation failed");
    abst->run(f);
    return false;
  }
  void print(llvm::raw_ostream &os, const llvm::Module *) const override {
    assert(abst && "allocation failed");
    abst->print(os);
  }
};

char Analyzer::ID = 0;
static llvm::RegisterPass<Analyzer> registerpass("stacksafe", "stacksafe",
                                                 false, true);
}  // namespace stacksafe
