#include "env.hpp"
#include <llvm/Support/raw_ostream.h>
#include "domain.hpp"
#include "json.hpp"
#include "value.hpp"

namespace stacksafe {

void Env::alloc(const Value& dst) {
  auto type = dst.type();
  if (type.is_pointer()) {
    auto sym = Symbol::create(type.pointee_type());
    heap_.insert(sym);
    stack_.insert(dst, sym);
  } else {
    stack_.insert(dst);
  }
}
void Env::store(const Value& src, const Value& dst) {
  auto source = stack_.get(src);
  auto target = stack_.get(dst);
  if (source && target) {
    for (auto& t : *target) {
      heap_.insert(t, *source);
    }
  }
}
void Env::load(const Value& dst, const Value& src) {
  if (auto ptr = stack_.get(src)) {
    for (auto& sym : *ptr) {
      if (auto source = heap_.get(sym)) {
        stack_.insert(dst, *source);
      }
    }
  }
}
void Env::binop(const Value& dst) { stack_.insert(dst); }
void to_json(Json& j, const Env& x) {
  j["stack"] = x.stack_;
  j["heap"] = x.heap_;
}

}  // namespace stacksafe
