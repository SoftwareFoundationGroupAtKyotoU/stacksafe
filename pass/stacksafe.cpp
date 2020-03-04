#include "stacksafe.hpp"
#include <llvm/IR/Module.h>
#include <llvm/Support/CommandLine.h>
#include <string>
#include "block.hpp"
#include "pointsto.hpp"
#include "state.hpp"
#include "tarjan.hpp"

namespace stacksafe {
namespace {
void endline(llvm::raw_ostream& os) {
  (os << "\n").flush();
}
constexpr auto stacksafe{"stacksafe"};
const llvm::cl::desc logfile_desc{"Print log"};
const llvm::cl::value_desc logfile_value{"filename"};
const llvm::cl::opt<std::string> logfile{"logfile", logfile_desc,
                                         logfile_value};
const llvm::cl::desc stopwatch_desc{"Measure the time of analysis"};
const llvm::cl::opt<bool> stopwatch{"stopwatch", stopwatch_desc};
}  // namespace

char StackSafe::ID = 0;
StackSafe::StackSafe() : llvm::ModulePass{ID} {}
bool StackSafe::runOnModule(llvm::Module& m) {
  for (const auto& f : m) {
    if (!f.isDeclaration()) {
      print_safe(llvm::outs(), analyze(f));
      endline(llvm::outs() << ": " << f.getName());
    }
  }
  return false;
}
void StackSafe::print(llvm::raw_ostream&, const llvm::Module*) const {}
void StackSafe::print_safe(llvm::raw_ostream& os, bool safe) const {
  const auto color = safe ? llvm::raw_ostream::GREEN : llvm::raw_ostream::RED;
  const auto str = safe ? "SAFE" : "UNSAFE";
  if (os.is_displayed()) {
    os.changeColor(color, true);
    os << str;
    os.resetColor();
  } else {
    os << str;
  }
}
bool StackSafe::analyze(const llvm::Function& f) const {
  State state{f};
  for (auto&& c : state) {
    PointsTo::analyze(c.graph(), c.blocks());
    state.transfer(c);
  }
  return state.is_safe();
}

static llvm::RegisterPass<StackSafe> registerpass{stacksafe, stacksafe, false,
                                                  true};

}  // namespace stacksafe
