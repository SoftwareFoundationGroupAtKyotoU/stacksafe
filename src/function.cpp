#include "function.hpp"
#include <llvm/IR/InstrTypes.h>
#include <llvm/IR/Module.h>
#include <memory>

namespace dataflow {
namespace function {
auto Scc::solve(const llvm::Module& m) -> std::vector<Component> {
  auto tarjan = std::make_unique<Scc>();
  for (const auto& f : m) {
    if (!f.isDeclaration()) {
      tarjan->push_back(&f);
    }
  }
  tarjan->run();
  const auto& result = tarjan->result();
  std::vector<Component> vec;
  for (const auto& v : result) {
    Component c;
    for (auto p = v.crbegin(); p != v.crend(); ++p) {
      if (auto f = llvm::dyn_cast<llvm::Function>(*p)) {
        c.push_back(f);
      }
    }
    vec.push_back(std::move(c));
  }
  return vec;
}
auto Scc::successors(Ptr p) const -> std::vector<Ptr> {
  std::vector<Ptr> vec;
  if (auto f = llvm::dyn_cast<llvm::Function>(p)) {
    for (const auto& b : *f) {
      for (const auto& i : b) {
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
