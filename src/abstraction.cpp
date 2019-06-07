#include "abstraction.hpp"
#include "interpret.hpp"
#include "visualize.hpp"
#include <llvm/IR/Function.h>
#include <llvm/Support/raw_ostream.h>

namespace stacksafe {
Abstraction::Abstraction(llvm::Function &F) {
  llvm::outs() << "name: " << F.getName() << endl;
  Env_ empty{factory_};
  for (auto &b : F.getBasicBlockList()) {
    map_.emplace(&b, empty);
  }
  auto entry = &F.getEntryBlock();
  todo_.insert(entry);
  auto &env = map_.at(entry);
  for (auto &a : F.args()) {
    if (auto reg = make_register(a)) {
      if (!env.init_argument(*reg)) {
        llvm::errs() << "Error: " << spaces(*reg) << "is not an argument"
                     << endl;
      }
    } else {
      llvm::errs() << "Error: unreachable" << endl;
    }
  }
}
void Abstraction::proceed() {
  while (!todo_.empty()) {
    auto front = todo_.begin();
    auto b = *front;
    todo_.erase(front);
    update(b);
  }
}
void Abstraction::update(BB b) {
  llvm::outs() << "label: %" << get_number(*b) << endl;
  const auto prev = interpret(*b, map_.at(b));
  llvm::outs() << endl;
  if (auto t = b->getTerminator()) {
    for (unsigned i = 0; i < t->getNumSuccessors(); ++i) {
      auto succ = t->getSuccessor(i);
      auto &next = map_.at(succ);
      if (!prev.subsetof(next)) {
        next.unify(prev);
        todo_.insert(succ);
      }
    }
  } else {
    llvm::errs() << "No terminator" << endl;
  }
}
} // namespace stacksafe
