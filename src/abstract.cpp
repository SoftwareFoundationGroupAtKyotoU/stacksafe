#include "abstract.hpp"
#include <llvm/IR/Function.h>
#include <llvm/Support/Format.h>
#include <llvm/Support/raw_ostream.h>
#include <chrono>
#include "interpreter.hpp"

namespace stacksafe {
namespace {
template <class Period>
class Stopwatch {
  using Clock = std::chrono::high_resolution_clock;
  using Duration = std::chrono::duration<double, Period>;
  double &elapsed_;
  decltype(Clock::now()) start_;

 public:
  explicit Stopwatch(double &elapsed)
      : elapsed_{elapsed}, start_{Clock::now()} {}
  ~Stopwatch() {
    auto end = Clock::now();
    Duration elapsed = end - start_;
    elapsed_ = elapsed.count();
  }
};
}  // namespace

Abstract::Abstract(const llvm::Function &f)
    : log_{f},
      blocks_{f, log_, error_},
      name_{f.getName().str()},
      elapsed_{0.0} {}
void Abstract::run(const llvm::Function &f) {
  using namespace std::chrono;
  {
    Stopwatch<std::milli> watch{elapsed_};
    interpret(f.getEntryBlock());
  }
}
void Abstract::print(llvm::raw_ostream &os) const {
  const auto safe = !error_.is_error();
  const auto color = safe ? llvm::raw_ostream::GREEN : llvm::raw_ostream::RED;
  const auto prefix = safe ? "SAFE" : "UNSAFE";
  const auto msg = llvm::format(": %s %fms\n", name_.c_str(), elapsed_);
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
  Interpreter i{log_, error_, blocks_.get(b)};
  i.visit(b);
  auto t = b.getTerminator();
  assert(t && "no terminator");
  for (unsigned j = 0; j < t->getNumSuccessors(); ++j) {
    if (error_.is_error()) {
      return;
    }
    const auto &next = *t->getSuccessor(j);
    if (blocks_.get(next).includes(i.env())) {
      continue;
    }
    blocks_.get(next).merge(i.env());
    interpret(next);
  }
}

}  // namespace stacksafe
