#include "abstract.hpp"
#include <llvm/IR/Function.h>
#include <llvm/Support/Format.h>
#include <llvm/Support/raw_ostream.h>
#include "interpreter.hpp"
#include "log.hpp"
#include "map.hpp"
#include "stopwatch.hpp"
#include "tarjan.hpp"

namespace stacksafe {
namespace {
unsigned count_call(const llvm::Function &f) {
  unsigned count = 0;
  for (const auto &b : f) {
    for (const auto &i : b) {
      if (llvm::isa<llvm::CallBase>(i)) {
        ++count;
      }
    }
  }
  return count;
}
}  // namespace

Abstract::Abstract(const llvm::Function &f)
    : func_{f}, depend_{f}, elapsed_{0.0} {}
void Abstract::interpret() {
  Log log{func_};
  {
    Stopwatch<std::milli> watch{elapsed_};
    auto scc = Tarjan::run(func_);
    for (auto &[c, m] : scc) {
      Interpreter i{log, depend_, depmap_, m};
      do {
        bool repeat = false;
        for (const auto &b : c) {
          if (i.visit(*b)) {
            repeat = true;
          }
          if (depend_.is_error()) {
            return;
          }
        }
        if (c.is_loop() && repeat) {
          continue;
        }
      } while (false);
      scc.transfer(c, m);
    }
  }
}
void Abstract::print(llvm::raw_ostream &os) const {
  const auto safe = !depend_.is_error();
  const auto color = safe ? llvm::raw_ostream::GREEN : llvm::raw_ostream::RED;
  const auto prefix = safe ? "SAFE" : "UNSAFE";
  const auto name = func_.getName().str();
  const auto insts = func_.getInstructionCount();
  const auto calls = count_call(func_);
  const auto msg =
      llvm::format(": %s %u %u %fms\n", name.c_str(), insts, calls, elapsed_);
  if (os.is_displayed()) {
    os.changeColor(color, true);
    os << prefix;
    os.resetColor();
  } else {
    os << prefix;
  }
  (os << msg).flush();
}
void Abstract::print_depend(llvm::raw_ostream &os) const {
  if (!depend_.is_error()) {
    os << "DEPEND: ";
    depend_.print(os);
  }
}

}  // namespace stacksafe
