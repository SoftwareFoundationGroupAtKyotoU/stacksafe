#include "environment.hpp"
#include <llvm/IR/Value.h>

namespace stacksafe {
  Env::Env(LocationFactory &factory)
    : factory_(factory) {
    auto g = LocationFactory::getGlobal();
    auto o = LocationFactory::getOutlive();
    just_added(heap_.add(g, g));
    just_added(heap_.add(o, LocationSet{{g, o}}));
  }
  bool Env::subsetof(const Env &rhs) const {
    return (heap_.subsetof(rhs.heap_) &&
            stack_.subsetof(rhs.stack_));
  }
  void Env::unify(const Env &rhs) {
    heap_.unify(rhs.heap_);
    stack_.unify(rhs.stack_);
  }
  void Env::print(llvm::raw_ostream &O) const {
    O << "heap: " << heap_ << endl;
    O << "stack: " << stack_ << endl;
  }

  bool Env::init_argument(const Register &dst) {
    if (auto &val = dst.get(); llvm::isa<llvm::Argument>(val)) {
      if (llvm::isa<llvm::PointerType>(val.getType())) {
        auto o = LocationFactory::getOutlive();
        auto g = LocationFactory::getGlobal();
        just_added(stack_.add(dst, LocationSet{{g, o}}));
      } else {
        just_added(stack_.add(dst));
      }
      return true;
    }
    return false;
  }
  bool Env::alloca(const Register &dst) {
    auto l = factory_.getLocal();
    just_added(heap_.add(l));
    just_added(stack_.add(dst, l));
    return true;
  }
  bool Env::store(const Value &src, const Register &dst) {
    if (auto val = to_symbols(src)) {
      if (auto ptr = stack_.get(dst)) {
        if (heap_.exists(*ptr)) {
          for (auto &each: *ptr) {
            just_added(!heap_.add(each, *val));
          }
          return true;
        }
      }
    }
    return false;
  }
  bool Env::load(const Register &dst, const Register &src) {
    if (auto ptr = stack_.get(src)) {
      if (heap_.exists(*ptr)) {
        for (auto &each: *ptr) {
          if (auto val = heap_.get(each)) {
            stack_.add(dst, *val);
          }
        }
        return true;
      }
    }
    return false;
  }
  bool Env::getelementptr(const Register &dst, const Register &src) {
    if (auto val = stack_.get(src)) {
      return just_added(stack_.add(dst, *val));
    }
    return false;
  }
  bool Env::binary(const Register &dst) {
    return just_added(stack_.add(dst));
  }
  bool Env::cast(const Register &dst, const Value &src) {
    if (llvm::isa<llvm::Constant>(src.get())) {
      return just_added(stack_.add(dst));
    } else if (auto reg = make_register(src)) {
      if (auto val = stack_.get(*reg)) {
        return just_added(stack_.add(dst, *val));
      }
    }
    return false;
  }
  bool Env::phi(const Register &dst, const Value &src) {
    if (llvm::isa<llvm::Constant>(src.get())) {
      return just_added(stack_.add(dst));
    } else if (auto reg = make_register(src)) {
      if (auto val = stack_.get(*reg)) {
        return just_added(stack_.add(dst, *val));
      }
    }
    return false;
  }
  bool Env::cmp(const Register &dst) {
    return !just_added(stack_.add(dst));
  }

  std::optional<LocationSet> Env::to_symbols(const Value &v) const {
    const auto ptr = &v.get();
    if (auto reg = make_register(v)) {
      return to_optional(stack_.get(*reg));
    } else if (llvm::isa<llvm::ConstantPointerNull>(ptr)) {
      return to_optional(heap_.get(LocationFactory::getGlobal()));
    } else if (llvm::isa<llvm::Constant>(ptr)) {
      return LocationSet{};
    } else {
      return std::nullopt;
    }
  }
  bool Env::just_added(bool cond) {
    if (cond) {
      llvm::errs() << "unexpected" << endl;
    }
    return cond;
  }

  Reachable::Reachable(Env &env)
    : heap_{env.heap_}, stack_{env.stack_} {
      reachable_.insert(LocationFactory::getGlobal());
    }
  bool Reachable::add(const Register &reg) {
    args_.insert(reg);
    if (auto next = stack_.get(reg)) {
      return add(*next);
    }
    return false;
  }
  bool Reachable::add(const LocationSet &locs) {
    reachable_.unify(locs);
    for (auto &loc: locs) {
      if (auto next = heap_.get(loc)) {
        if (!next->subsetof(reachable_) && add(*next)) {
          continue;
        }
      }
      return false;
    }
    return true;
  }
}
