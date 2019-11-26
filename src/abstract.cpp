#include "abstract.hpp"
#include <llvm/IR/Function.h>
#include <llvm/Support/Format.h>
#include <llvm/Support/raw_ostream.h>
#include "graph.hpp"
#include "interpreter.hpp"
#include "log.hpp"
#include "stopwatch.hpp"

namespace stacksafe {

Abstract::Abstract(const llvm::Function &f) : func_{f}, elapsed_{0.0} {}
void Abstract::interpret() {
  Log log{func_};
  {
    Stopwatch<std::milli> watch{elapsed_};
    auto scc = Scc::decompose(func_);
    while (!scc.empty()) {
      auto ptr = scc.top();
      scc.pop();
      if (ptr->is_loop()) {
        Interpreter i{log, error_, ptr->map()};
        bool repeat = true;
        while (repeat) {
          repeat = false;
          for (const auto &b : *ptr) {
            if (i.visit(*b)) {
              repeat = true;
            }
          }
        }
        ptr->convey();
      } else {
        Interpreter i{log, error_, ptr->map()};
        for (const auto &b : *ptr) {
          i.visit(*b);
        }
        ptr->convey();
      }
    }
  }
}
void Abstract::print(llvm::raw_ostream &os) const {
  const auto safe = !error_.is_error();
  const auto color = safe ? llvm::raw_ostream::GREEN : llvm::raw_ostream::RED;
  const auto prefix = safe ? "SAFE" : "UNSAFE";
  const auto name = func_.getName().str();
  const auto msg = llvm::format(": %s %fms\n", name.c_str(), elapsed_);
  if (os.is_displayed()) {
    os.changeColor(color, true);
    os << prefix;
    os.resetColor();
  } else {
    os << prefix;
  }
  (os << msg).flush();
}

}  // namespace stacksafe
