#include "abstract.hpp"
#include <llvm/IR/Function.h>
#include <llvm/Support/Format.h>
#include <llvm/Support/raw_ostream.h>
#include "block.hpp"
#include "interpreter.hpp"
#include "log.hpp"
#include "map.hpp"
#include "pointsto.hpp"
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
    for (const auto &[b, m] : Tarjan::run(func_)) {
      state_.emplace_back(b);
    }
    for (auto &&c : state_) {
      PointsTo::analyze(c.graph(), c.blocks());
      state_.transfer(c);
    }
  }
  {
    Stopwatch<std::milli> watch{elapsed_};
    auto scc = Tarjan::run(func_);
    for (auto &[c, m] : scc) {
      Interpreter i{log, depend_, depmap_, m};
      if (c.is_loop()) {
        bool repeat = true;
        while (std::exchange(repeat, false)) {
          for (const auto &b : c) {
            if (i.visit(*b)) {
              repeat = true;
            }
            if (depend_.is_error()) {
              return;
            }
          }
        }
      } else {
        for (const auto &b : c) {
          i.visit(*b);
          if (depend_.is_error()) {
            return;
          }
        }
      }
      scc.transfer(c, m);
    }
  }
}
void Abstract::print(llvm::raw_ostream &os) const {
  const auto safe = state_.is_safe();
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
