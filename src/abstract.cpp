#include "abstract.hpp"
#include "interpret.hpp"
#include "json.hpp"

namespace stacksafe {

void Abstraction::interpret(llvm::Function& f) {
  Interpret interpreter(*this);
  interpreter.visit(f);
}
void Abstraction::argument(const Register& arg) {
  stack_.try_emplace(arg, Symbol::create());
}
void Abstraction::alloc(const Register& r) {
  stack_.try_emplace(r, Symbol::create());
}
void Abstraction::store(const Register& src, const Register& dst) {
  auto source = stack_.find(src);
  auto target = stack_.find(dst);
  auto end = stack_.end();
  if (source != end && target != end) {
    heap_.try_emplace(target->second, source->second);
  }
}
void Abstraction::show() const {
  Json j;
  j["stack"] = stack_;
  j["heap"] = heap_;
  llvm::errs() << j.dump(2) << "\n";
}

}  // namespace stacksafe
