#include "environment.hpp"
#include <llvm/IR/Value.h>

namespace stacksafe {
  Env::Env(LocationFactory &factory)
    : factory_(factory) {
    auto g = factory_.getGlobal();
    auto o = factory_.getOutlive();
    heap_.add(g, g);
    heap_.add(o, LocationSet{{g, o}});
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
        auto o = factory_.getOutlive();
        auto g = factory_.getGlobal();
        stack_.add(dst, LocationSet{{g, o}});
      }
      return true;
    }
    return false;
  }
  bool Env::alloca(const Register &dst) {
    auto l = factory_.getLocal();
    heap_.add(l);
    stack_.add(dst, l);
    return true;
  }
  bool Env::store(const Value &src, const Register &dst) {
    using std::begin;
    using std::end;
    if (auto val = to_symbols(src)) {
      if (auto ptr = stack_.get(dst)) {
        if (heap_.exists(*ptr)) {
          for (auto &each: *ptr) {
            heap_.add(each, *val);
          }
          return true;
        }
      }
    }
    return false;
  }
  bool Env::load(const Register &dst, const Register &src) {
    using std::begin;
    using std::end;
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
      stack_.add(dst, *val);
      return true;
    }
    return false;
  }

  std::optional<LocationSet> Env::to_symbols(const Value &v) const {
    const auto ptr = &v.get();
    if (auto reg = make_register(*ptr)) {
      return to_optional(stack_.get(*reg));
    } else if (llvm::isa<llvm::ConstantPointerNull>(ptr)) {
      return to_optional(heap_.get(factory_.getGlobal()));
    } else if (llvm::isa<llvm::Constant>(ptr)) {
      return LocationSet{};
    } else {
      return std::nullopt;
    }
  }
  void Env::just_added(bool cond) {
    if (cond) {
      llvm::errs() << "unexpected" << endl;
    }
  }

  Environment::Environment(LocationFactory &factory)
    : factory_(factory) {
    auto g = factory.getGlobal();
    auto o = factory.getOutlive();
    if (auto locs = heap_.init(g)) {
      locs->get().insert(g);
    }
    if (auto locs = heap_.init(o)) {
      locs->get().insert(g);
      locs->get().insert(o);
    }
  }
  bool Environment::subsetof(const Environment &rhs) const {
    return heap_.subsetof(rhs.heap_) && stack_.subsetof(rhs.stack_);
  }
  void Environment::unify(const Environment &rhs) {
    heap_.unify(rhs.heap_);
    stack_.unify(rhs.stack_);
  }
  void Environment::print(llvm::raw_ostream &O) const {
    O << "heap: " << heap_ << endl;
    O << "stack: " << stack_ << endl;
  }
  std::optional<LocationSet> Environment::at(const Value &val) {
    auto ptr = &val.get();
    if (auto reg = make_register(*ptr)) {
      if (stack_.exists(*reg)) {
        return stack_.at(*reg)->get();
      } else {
        return std::nullopt;
      }
    } else if (llvm::isa<llvm::ConstantPointerNull>(ptr)) {
      LocationSet locs;
      locs.insert(factory_.getGlobal());
      return locs;
    } else if (llvm::isa<llvm::Constant>(ptr)) {
      return LocationSet{};
    } else {
      return std::nullopt;
    }
  }
  bool Environment::argument(const Register &dst) {
    if (auto &val = dst.get(); llvm::isa<llvm::Argument>(val)) {
      auto target = stack_.ensure(dst);
      if (llvm::isa<llvm::PointerType>(val.getType())) {
        target.insert(factory_.getOutlive());
      }
      return true;
    }
    return false;
  }
  bool Environment::alloca(const Register &dst) {
    auto target = stack_.ensure(dst);
    auto loc = factory_.getLocal();
    target.insert(loc);
    heap_.ensure(loc);
    return true;
  }
  bool Environment::load(const Register &dst, const Register &src) {
    if (auto target = stack_.init(dst)) {
      if (auto locs = stack_.at(src)) {
        for (auto &loc: locs->get()) {
          if (auto src_loc = heap_.at(loc)) {
            target->get().unify(*src_loc);
          } else {
            return false;
          }
        }
        return true;
      }
    }
    return false;
  }
  bool Environment::store(const Value &src, const Register &dst) {
    if (auto val = at(src)) {
      if (auto ptr = stack_.at(dst)) {
        for (auto &each: ptr->get()) {
          if (auto target = heap_.at(each)) {
            target->get().unify(*val);
          } else {
            return false;
          }
        }
        return true;
      }
    }
    return false;
  }
  bool Environment::getelementptr(const Register &dst, const Register &src) {
    if (auto target = stack_.init(dst)) {
      if (auto locs = stack_.at(src)) {
        target->get().unify(locs->get());
        return true;
      }
    }
    return false;
  }
  bool Environment::binary(const Register &dst) {
    if (stack_.init(dst)) {
      return true;
    }
    return false;
  }
  bool Environment::cast(const Register &dst, const Register &src) {
    if (auto target = stack_.init(dst)) {
      if (auto source = stack_.at(src)) {
        target->get().unify(source->get());
        return true;
      }
    }
    return false;
  }
  bool Environment::phi(const Register &dst, const Register &src) {
    if (auto target = stack_.init(dst)) {
      if (auto source = stack_.at(src)) {
        target->get().unify(source->get());
        return true;
      }
    }
    return false;
  }
}
