#include "abstract.hpp"
#include <llvm/IR/Function.h>
#include <llvm/Support/Debug.h>
#include "debug.hpp"
#include "json.hpp"
#include "symbol.hpp"
#include "utility.hpp"

namespace stacksafe {

Abstract::Abstract(const llvm::Function& f)
    : blocks_{f}, func_{f}, safe_{false} {
#define DEBUG_TYPE "log"
  LLVM_DEBUG(log_ = std::make_unique<Log>(f));
#undef DEBUG_TYPE
  Symbol::reset();
}
Abstract::~Abstract() = default;
auto Abstract::blocks() const -> const Blocks& {
  return blocks_;
}
void Abstract::interpret() {
  interpret(&func_.getEntryBlock());
}
void Abstract::verify() {
  for (auto& b : func_) {
    if (!blocks_.verify(&b)) {
      return;
    }
  }
  safe_ = true;
}
void Abstract::print(llvm::raw_ostream& os) const {
  auto name = func_.getName();
  if (safe_) {
    os.changeColor(llvm::raw_ostream::GREEN);
    endline(os << "Safe: " << name);
  } else {
    os.changeColor(llvm::raw_ostream::RED, true);
    endline(os << "Unsafe: " << name);
  }
}
void Abstract::interpret(const llvm::BasicBlock* b) {
  auto result = blocks_.interpret(b);
#define DEBUG_TYPE "log"
  LLVM_DEBUG(blocks_.print(*log_, b, result));
#undef DEBUG_TYPE
  auto t = b->getTerminator();
  assert(t && "invalid basicblock");
  for (unsigned i = 0; i < t->getNumSuccessors(); ++i) {
    auto next = t->getSuccessor(i);
    if (blocks_.update(next, result)) {
      interpret(next);
    }
  }
}
void to_json(Json& j, const Abstract& x) {
  Json::object_t obj;
  for (auto& [k, v] : x.blocks()) {
    assert(k && "unknown basicblock");
    obj[Value::make(*k).repr()] = v;
  }
  j = obj;
}

}  // namespace stacksafe
