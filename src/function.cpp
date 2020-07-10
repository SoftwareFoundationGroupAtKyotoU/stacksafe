#include "function.hpp"
#include <llvm/IR/InstrTypes.h>
#include <llvm/IR/Module.h>
#include <memory>

namespace dataflow {
namespace function {
std::vector<Component> Scc::solve(const llvm::Module &m) {
  auto tarjan = std::make_unique<Scc>();
  for (const auto &f : m) {
    if (!f.isDeclaration()) {
      tarjan->push_back(&f);
    }
  }
  const auto result = tarjan->apply();
  std::vector<Component> vec;
  for (const auto &v : result) {
    Component c;
    for (const auto &p : v) {
      c.push_back(static_cast<const llvm::Function *>(p));
    }
    vec.push_back(std::move(c));
  }
  return vec;
}
auto Scc::successors(Ptr p) const -> Vec {
  Vec vec;
  if (auto f = static_cast<const llvm::Function *>(p)) {
    for (const auto &b : *f) {
      for (const auto &i : b) {
        if (auto c = llvm::dyn_cast<llvm::CallBase>(&i)) {
          if (auto d = c->getCalledFunction()) {
            if (!d->isDeclaration()) {
              vec.push_back(d);
            }
          }
        }
      }
    }
  }
  return vec;
}
}  // namespace function
}  // namespace dataflow
