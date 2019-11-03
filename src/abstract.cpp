#include "abstract.hpp"
#include <llvm/IR/Function.h>
#include "json.hpp"
#include "utility.hpp"

namespace stacksafe {

Abstract::Abstract(const llvm::Function &f)
    : log_{f}, blocks_{f, log_}, func_{f}, safe_{true} {}
auto Abstract::blocks() const -> const Blocks & {
  return blocks_;
}
void Abstract::interpret() {
  interpret(func_.getEntryBlock());
  if (blocks_.is_error()) {
    safe_ = false;
  }
}
void Abstract::print(llvm::raw_ostream &os) const {
  if (os.is_displayed()) {
    if (safe_) {
      os.changeColor(llvm::raw_ostream::GREEN);
    } else {
      os.changeColor(llvm::raw_ostream::RED, true);
    }
  }
  os << (safe_ ? "Safe" : "Unsafe") << ": " << func_.getName();
  endline(os, true);
}
void Abstract::interpret(const llvm::BasicBlock &b) {
  log_.print_hr2().print(b).print_hr().print_nl();
  auto result = blocks_.interpret(b);
  auto t = b.getTerminator();
  assert(t && "invalid basicblock");
  for (unsigned i = 0; i < t->getNumSuccessors(); ++i) {
    const auto &next = *t->getSuccessor(i);
    if (blocks_.update(next, result)) {
      interpret(next);
    }
  }
}
void to_json(Json &j, const Abstract &x) {
  Json::object_t obj;
  for (const auto &[k, v] : x.blocks()) {
    assert(k && "unknown basicblock");
    obj[to_str(*k)] = v;
  }
  j = obj;
}

}  // namespace stacksafe
