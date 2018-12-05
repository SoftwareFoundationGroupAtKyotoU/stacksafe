#include "environment.hpp"
#include <llvm/IR/Value.h>

namespace stacksafe {
  bool LocationSet::subsetof(const LocationSet &rhs) const {
    using std::begin;
    using std::end;
    return std::includes(begin(rhs), end(rhs), begin(*this), end(*this));
  }
  void LocationSet::unify(const LocationSet &rhs) {
    using std::begin;
    using std::end;
    insert(begin(rhs), end(rhs));
  }
  void LocationSet::print(llvm::raw_ostream &O) const {
    O << set_like(make_manip_seq(*this));
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
  bool Environment::initArg(const Register &key) {
    if (auto &val = key.get(); llvm::isa<llvm::Argument>(val)) {
      if (auto target = stack_.init(key)) {
        if (llvm::isa<llvm::PointerType>(val.getType())) {
          target->get().insert(factory_.getOutlive());
        }
        return true;
      }
    } else {
      llvm::errs() << "Error: " << spaces(make_manip(key))
                   << "is not an argument" << endl;
    }
    return false;
  }
  bool Environment::alloca(const Register &key) {
    if (auto target = stack_.init(key)) {
      auto loc = factory_.getLocal();
      target->get().insert(loc);
      if (heap_.init(loc)) {
        return true;
      }
    }
    return false;
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
  bool Environment::store(const Register &src, const Register &dst) {
    if (auto val = stack_.at(src)) {
      if (auto ptr = stack_.at(dst)) {
        for (auto &each: ptr->get()) {
          if (auto target = heap_.at(each)) {
            target->get().unify(val->get());
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
