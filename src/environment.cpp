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
  LocationSet &Environment::init(const Location &key) {
    return heap_.init(key)->get();
  }
  LocationSet &Environment::init(const Register &key) {
    return stack_.init(key)->get();
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
  bool Environment::alloc(const Register &key) {
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
      if (auto locs = stack_.get(src)) {
        for (auto &loc: locs->get()) {
          if (auto src_loc = heap_.get(loc)) {
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
  OptRef<LocationSet> Environment::get(const Location &key) {
    return heap_.get(key);
  }
  OptRef<LocationSet> Environment::get(const Register &key) {
    return stack_.get(key);
  }
  bool Environment::exists(const Location &key) {
    return heap_.count(key) != 0;
  }
  bool Environment::exists(const Register &key) {
    return stack_.count(key) != 0;
  }
}
