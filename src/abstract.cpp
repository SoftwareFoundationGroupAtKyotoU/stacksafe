#include "abstract.hpp"
#include "interpret.hpp"
#include "json.hpp"

namespace stacksafe {

void Abstraction::interpret(llvm::Function& f) {
  Interpret interpreter(*this);
  interpreter.visit(f);
}
void Abstraction::argument(const Register& arg) {
  stack_.insert(arg, Symbol::create());
}
void Abstraction::alloc(const Register& r) {
  stack_.insert(r, Symbol::create());
}
void Abstraction::store(const Register& src, const Register& dst) {
  auto source = stack_.get(src);
  auto target = stack_.get(dst);
  if (source && target) {
    for (auto& t : *target) {
      heap_.insert(t, *source);
    }
  }
}
void Abstraction::show() const {
  Json j;
  j["stack"] = stack_;
  j["heap"] = heap_;
  llvm::errs() << j.dump(2) << "\n";
}

}  // namespace stacksafe
