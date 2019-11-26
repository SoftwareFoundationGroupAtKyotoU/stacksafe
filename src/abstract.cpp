#include "abstract.hpp"
#include <llvm/IR/Function.h>
#include <llvm/Support/Format.h>
#include <llvm/Support/raw_ostream.h>
#include "domain.hpp"
#include "env.hpp"
#include "graph.hpp"
#include "interpreter.hpp"
#include "map.hpp"
#include "stopwatch.hpp"

namespace stacksafe {
namespace {
std::size_t estimate(const llvm::Function &f) {
  std::size_t count = 0;
  for (const auto &b : f) {
    count += b.size();
  }
  return count;
}
}  // namespace

Abstract::Abstract(const llvm::Function &f)
    : log_{f}, pool_{estimate(f)}, name_{f.getName().str()}, elapsed_{0.0} {
  for (const auto &b : f) {
    blocks_.try_emplace(&b, pool_.make_env());
  }
}
void Abstract::run_scc(const llvm::Function &f) {
  {
    Stopwatch<std::milli> watch{elapsed_};
    auto scc = Scc::decompose(f);
    while (!scc.empty()) {
      auto ptr = scc.top();
      scc.pop();
      if (ptr->is_loop()) {
        Interpreter i{log_, error_, ptr->map()};
        bool repeat = true;
        while (repeat) {
          repeat = false;
          for (const auto &b : *ptr) {
            i.reset();
            i.visit(*b);
            if (i.diff()) {
              repeat = true;
            }
          }
        }
        ptr->convey();
      } else {
        Interpreter i{log_, error_, ptr->map()};
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
  Map m = pool_.make_map();
  MutableEnv env{get(b), pool_};
  Interpreter i{log_, error_, m};
  i.visit(b);
  const auto &prev = env.finish(pool_);
  const auto t = b.getTerminator();
  assert(t && "no terminator");
  for (unsigned j = 0; j < t->getNumSuccessors(); ++j) {
    if (error_.is_error()) {
      return;
    }
    const auto &succ = *t->getSuccessor(j);
    auto &next = get(succ);
    if (!next.includes(prev)) {
      next.merge(prev);
      interpret(succ);
    }
  }
}
Env &Abstract::get(const llvm::BasicBlock &b) {
  auto it = blocks_.find(&b);
  assert(it != blocks_.end() && "unknown basicblock");
  return it->second;
}

}  // namespace stacksafe
