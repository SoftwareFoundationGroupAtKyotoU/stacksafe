#include "abstraction.hpp"
#include <llvm/IR/Function.h>
#include <llvm/Support/raw_ostream.h>
#include "interpret.hpp"
#include "visualize.hpp"

namespace stacksafe {
void Abstraction::initialize() {
  auto empty = Env_{factory_};
  for (auto &b : func_.getBasicBlockList()) {
    map_.emplace(&b, empty);
  }
  init_args();
}
void Abstraction::init_args() {
  auto entry = &func_.getEntryBlock();
  todo_.insert(entry);
  auto &env = map_.at(entry);
  for (auto &a : func_.args()) {
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
Abstraction::Abstraction(llvm::Function &F) : func_{F} {
  llvm::outs() << "name: " << F.getName() << endl;
  initialize();
  while (!todo_.empty()) {
    auto front = todo_.begin();
    auto b = *front;
    todo_.erase(front);
    update(b);
  }
}
}  // namespace stacksafe
