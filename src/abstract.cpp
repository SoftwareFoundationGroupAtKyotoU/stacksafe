#include "abstract.hpp"
#include <llvm/IR/Function.h>
#include "utility.hpp"

namespace stacksafe {

Abstract::Abstract(const llvm::Function &f)
    : blocks_{f}, name_{f.getName().str()} {}
void Abstract::run(const llvm::Function &f) {
  interpret(f.getEntryBlock());
}
void Abstract::print(llvm::raw_ostream &os) const {
  if (os.is_displayed()) {
    if (safe_) {
      os.changeColor(llvm::raw_ostream::GREEN);
    } else {
      os.changeColor(llvm::raw_ostream::RED, true);
    }
  }
  os << (safe_ ? "SAFE" : "UNSAFE") << ": " << name_;
  endline(os, true);
}
void Abstract::interpret(const llvm::BasicBlock &b) {
  if (!safe_) {
    return;
  }
  if (auto result = blocks_.interpret(b)) {
    auto t = b.getTerminator();
    assert(t && "no terminator");
    for (unsigned i = 0; i < t->getNumSuccessors(); ++i) {
      const auto &next = *t->getSuccessor(i);
      if (blocks_.update(next, *result)) {
        interpret(next);
      }
    }
  } else {
    safe_.unsafe();
  }
}

}  // namespace stacksafe
