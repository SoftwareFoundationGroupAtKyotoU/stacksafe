#include "abstraction.hpp"
#include "interpret.hpp"
#include "visualize.hpp"
#include <llvm/IR/Function.h>
#include <llvm/Support/raw_ostream.h>

namespace stacksafe {
  Abstraction::Abstraction(llvm::Function &F) {
    Env empty(factory_);
    for (auto &b : F.getBasicBlockList()) {
      map_.emplace(&b, empty);
    }
    auto entry = &F.getEntryBlock();
    todo_.push(entry);
    for (auto &a: F.args()) {
      if (auto reg = make_register(a)) {
        if (!map_.at(entry).init_argument(*reg)) {
          llvm::errs() << "Error: " << spaces(make_manip(*reg))
                       << "is not an argument" << endl;
        }
      } else {
        llvm::errs() << "Error: unreachable" << endl;
      }
    }
  }
  void Abstraction::proceed() {
    while (!todo_.empty()) {
      auto b = todo_.front();
      todo_.pop();
      update(b);
    }
  }
  void Abstraction::update(BB b) {
    auto &env = map_.at(b);
    Interpret interpret(env);
    for (auto &i : b->getInstList()) {
      if (!interpret.visit(i)) {
        llvm::errs() << "Error: something wrong happens" << endl;
      }
    }
    if (auto t = b->getTerminator()) {
      for (unsigned i = 0; i < t->getNumSuccessors(); ++i) {
        auto succ = t->getSuccessor(i);
        auto &next = map_.at(succ);
        if (!env.subsetof(next)) {
          next.unify(env);
          todo_.push(succ);
        }
      }
    }
  }
}
