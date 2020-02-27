#include "stacksafe.hpp"
#include "block.hpp"
#include "pointsto.hpp"
#include "state.hpp"
#include "tarjan.hpp"

namespace stacksafe {
namespace {
constexpr auto stacksafe{"stacksafe"};
}

char StackSafe::ID = 0;
StackSafe::StackSafe() : llvm::ModulePass{ID} {}
bool StackSafe::runOnModule(llvm::Module&) {
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
