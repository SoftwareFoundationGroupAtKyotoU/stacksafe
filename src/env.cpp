#include "env.hpp"
#include <llvm/IR/Value.h>
#include <llvm/Support/raw_ostream.h>
#include "json.hpp"
#include "register.hpp"

namespace stacksafe {

void Env::alloc(const Register& dst) {
  auto sym = Symbol::create(dst.get_type());
  heap_.insert(sym, Domain{});
  stack_.insert(dst, sym);
}
void Env::store(const Register& src, const Register& dst) {
  auto source = stack_.get(src);
  auto target = stack_.get(dst);
  if (source && target) {
    for (auto& t : *target) {
      heap_.insert(t, *source);
    }
  }
}
void Env::load(const Register& dst, const Register& src) {
  if (auto ptr = stack_.get(src)) {
    for (auto& sym : *ptr) {
      if (auto source = heap_.get(sym)) {
        stack_.insert(dst, *source);
      }
    }
  }
}
void Env::show() const {
  Json j;
  j["stack"] = stack_;
  j["heap"] = heap_;
  llvm::errs() << j.dump(2) << "\n";
}

}  // namespace stacksafe
