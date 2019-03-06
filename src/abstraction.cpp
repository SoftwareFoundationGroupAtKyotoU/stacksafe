#include "abstraction.hpp"
#include "interpret.hpp"
#include "visualize.hpp"
#include <llvm/IR/Function.h>
#include <llvm/Support/raw_ostream.h>

namespace stacksafe {
  Abstraction::Abstraction(llvm::Function &F) {
    Env empty{factory_};
    for (auto &b: F.getBasicBlockList()) {
      map_.emplace(&b, empty);
    }
    auto entry = &F.getEntryBlock();
    todo_.push(entry);
    auto &env = map_.at(entry);
    for (auto &a: F.args()) {
      if (auto reg = make_register(a)) {
        if (!env.init_argument(*reg)) {
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
    auto label = make_manip("label %", get_number(*b), endl);
    llvm::outs() << label;
    const auto prev = interpret(*b, map_.at(b));
    llvm::outs() << label << endl;
    if (auto t = b->getTerminator()) {
      for (unsigned i = 0; i < t->getNumSuccessors(); ++i) {
        auto succ = t->getSuccessor(i);
        auto &next = map_.at(succ);
        if (!prev.subsetof(next)) {
          next.unify(prev);
          todo_.push(succ);
        }
      }
    } else {
      llvm::errs() << "No terminator" << endl;
    }
  }
}
