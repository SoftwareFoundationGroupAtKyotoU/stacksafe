#include "interpretation.hpp"
#include "visualize.hpp"
#include <llvm/IR/Function.h>
#include <llvm/Support/raw_ostream.h>

namespace stacksafe {
  Interpretation::Interpretation(llvm::Function &F) {
    Env empty(factory_);
    for (auto &b : F.getBasicBlockList()) {
      map_.emplace(&b, empty);
    }
    auto entry = &F.getEntryBlock();
    todo_.push(entry);
    for (auto &a: F.args()) {
      if (auto reg = make_register(a)) {
        if (!map_.at(entry).argument(*reg)) {
          llvm::errs() << "Error: " << spaces(make_manip(*reg))
                       << "is not an argument" << endl;
        }
      } else {
        llvm::errs() << "Error: unreachable" << endl;
      }
    }
  }
}
