#include "abstract.hpp"
#include <llvm/IR/Function.h>
#include <llvm/Support/Format.h>
#include <llvm/Support/raw_ostream.h>
#include "domain.hpp"
#include "env.hpp"
#include "interpreter.hpp"
#include "map.hpp"
#include "stopwatch.hpp"

namespace stacksafe {

Abstract::Abstract(const llvm::Function &f)
    : log_{f}, name_{f.getName().str()}, elapsed_{0.0} {
  for (const auto &b : f) {
    blocks_.try_emplace(&b);
  }
}
void Abstract::run(const llvm::Function &f) {
  using namespace std::chrono;
  {
    Stopwatch<std::milli> watch{elapsed_};
    const auto &entry = f.getEntryBlock();
    MutableEnv env{get(entry), pool_.add(Map{})};
    for (const auto &[key, val] : Map{f}) {
      Domain dom;
      dom.insert(val);
      env.insert(key, dom);
    }
    get(entry).merge(env.env());
    interpret(entry);
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
  MutableEnv env{get(b), pool_.add(Map{})};
  Interpreter i{log_, error_, Map{}, env};
  i.visit(b);
  env.finish(pool_);
  const auto &prev = env.env();
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
