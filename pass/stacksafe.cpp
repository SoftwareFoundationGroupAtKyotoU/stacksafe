#include "stacksafe.hpp"
#include <llvm/IR/Module.h>
#include "block.hpp"
#include "pointsto.hpp"
#include "state.hpp"
#include "tarjan.hpp"

namespace stacksafe {
namespace {
void endline(llvm::raw_ostream& os) {
  (os << "\n").flush();
}
constexpr auto stacksafe{"stacksafe"};
}  // namespace

char StackSafe::ID = 0;
StackSafe::StackSafe() : llvm::ModulePass{ID} {}
bool StackSafe::runOnModule(llvm::Module& m) {
  for (const auto& f : m) {
    if (!f.isDeclaration()) {
      const auto safe = analyze(f);
      const auto prefix = safe ? "SAFE" : "UNSAFE";
      endline(llvm::outs() << prefix << ": " << f.getName());
    }
  }
  return false;
}
void StackSafe::print(llvm::raw_ostream&, const llvm::Module*) const {}
bool StackSafe::analyze(const llvm::Function& f) const {
  State state;
  for (const auto& [c, m] : Tarjan::run(f)) {
    state.emplace_back(c);
  }
  for (auto&& c : state) {
    PointsTo::analyze(c.graph(), c.blocks());
    state.transfer(c);
  }
  return state.is_safe();
}

static llvm::RegisterPass<StackSafe> registerpass(stacksafe, stacksafe, false,
                                                  true);

}  // namespace stacksafe
