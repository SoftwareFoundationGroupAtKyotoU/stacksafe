#include "abstract.hpp"
#include <llvm/IR/Function.h>
#include "utility.hpp"

namespace stacksafe {

Abstract::Abstract(const llvm::Function &f)
    : log_{f}, blocks_{f, log_}, func_{f}, safe_{true} {}
void Abstract::run() {
  interpret(func_.getEntryBlock());
  if (blocks_.is_error()) {
    set_unsafe();
  }
}
void Abstract::print(llvm::raw_ostream &os) const {
  if (os.is_displayed()) {
    if (is_safe()) {
      os.changeColor(llvm::raw_ostream::GREEN);
    } else {
      os.changeColor(llvm::raw_ostream::RED, true);
    }
  }
  os << (is_safe() ? "SAFE" : "UNSAFE") << ": " << func_.getName();
  endline(os, true);
}
void Abstract::interpret(const llvm::BasicBlock &b) {
  if (!is_safe()) {
    return;
  }
  log_.print_hr2().print(b).print_hr().print_nl();
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
    set_unsafe();
  }
}
bool Abstract::is_safe() const {
  return safe_;
}
void Abstract::set_unsafe() {
  safe_ = false;
}

}  // namespace stacksafe
