#include "dataflow.hpp"
#include <llvm/IR/Function.h>
#include <llvm/Support/raw_ostream.h>
#include "function.hpp"
#include "state.hpp"

namespace dataflow {
namespace {
using RegisterPass = llvm::RegisterPass<DataFlow>;
static RegisterPass dataflow{"dataflow", "dataflow", true, true};
}  // namespace

char DataFlow::ID = 0;
DataFlow::DataFlow() : llvm::ModulePass{ID} {}
bool DataFlow::runOnModule(llvm::Module &m) {
  const auto hr = "----------------";
  auto scc = function::Scc::solve(m);
  for (const auto &c : scc) {
    for (const auto &f : c) {
      llvm::outs() << f->getName() << "\n";
      State state;
      for (const auto &b : *f) {
        state.transfer(b);
      }
    }
    llvm::outs() << hr << "\n";
  }
  return false;
}
void DataFlow::print(llvm::raw_ostream &, const llvm::Module *) const {}
}  // namespace dataflow
