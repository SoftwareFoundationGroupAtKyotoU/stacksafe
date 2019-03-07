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
        for (auto &each: *ptr) {
          if (heap_.add(each, *val)) {
            continue;
          }
          return false;
        }
        return true;
      }
    }
    return false;
  }
  bool Env::load(const Register &dst, const Register &src) {
    if (auto ptr = stack_.get(src)) {
      for (auto &each: *ptr) {
        if (auto val = heap_.get(each)) {
          stack_.add(dst, *val);
          continue;
        }
        return false;
      }
      return true;
    }
    return false;
  }
  bool Env::getelementptr(const Register &dst, const Register &src) {
    if (auto val = stack_.get(src)) {
      stack_.add(dst, *val);
      return true;
    }
    return false;
  }
  bool Env::binary(const Register &dst) {
    stack_.add(dst);
    return true;
  }
  bool Env::cast(const Register &dst, const Value &src) {
    if (llvm::isa<llvm::Constant>(src.get())) {
      stack_.add(dst);
      return true;
    } else if (auto reg = make_register(src)) {
      if (auto val = stack_.get(*reg)) {
        stack_.add(dst, *val);
        return true;
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
    stack_.add(dst);
    return true;
  }
  bool Env::call(const UserRange &args, std::optional<Register> dst) {
    LocationSet reachs{{LocationFactory::getGlobal()}};
    for (auto &arg: args) {
      if (auto reg = make_register(*arg.get())) {
        if (auto next = stack_.get(*reg)) {
          if (reach(*next, reachs)) {
            continue;
          }
        }
        return false;
      }
    }
    for (auto &r: reachs) {
      if (heap_.add(r, reachs)) {
        continue;
      }
      return false;
    }
    if (dst) {
      return !stack_.add(*dst, reachs);
    }
    return true;
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
  bool Env::reach(const LocationSet &locs, LocationSet &reachs) const {
    reachs.unify(locs);
    for (auto &loc: locs) {
      if (auto next = heap_.get(loc)) {
        if (next->subsetof(reachs) || reach(*next, reachs)) {
          continue;
        }
      }
      return false;
    }
    return true;
  }
  bool Env::just_added(bool cond) {
    if (cond) {
      llvm::errs() << "unexpected" << endl;
    }
    return cond;
  }
}
