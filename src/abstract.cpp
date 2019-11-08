#include "abstract.hpp"
#include <llvm/IR/Function.h>
#include <llvm/Support/Format.h>
#include <llvm/Support/raw_ostream.h>
#include <chrono>

namespace stacksafe {

Abstract::Abstract(const llvm::Function &f)
    : blocks_{f}, name_{f.getName().str()}, elapsed_{0.0}, count_{0} {}
void Abstract::run(const llvm::Function &f) {
  using namespace std::chrono;
  auto start = high_resolution_clock::now();
  interpret(f.getEntryBlock());
  auto end = high_resolution_clock::now();
  duration<double, std::milli> elapsed = end - start;
  elapsed_ = elapsed.count();
  for (const auto &b : f) {
    count_ += b.size();
  }
}
void Abstract::print(llvm::raw_ostream &os) const {
  const auto color = safe_ ? llvm::raw_ostream::GREEN : llvm::raw_ostream::RED;
  const auto prefix = safe_ ? "SAFE" : "UNSAFE";
  const auto msg =
      llvm::format(": %s %lu %fms\n", name_.c_str(), count_, elapsed_);
  if (os.is_displayed()) {
    os.changeColor(color, true);
    os << prefix;
    os.resetColor();
  } else {
    os << prefix;
  }
  (os << msg).flush();
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
