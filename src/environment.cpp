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
    return heap_.init(key);
  }
  LocationSet &Environment::init(const Register &key) {
    return stack_.init(key);
  }
  void Environment::init(LocationFactory &factory, const Register &key) {
    auto &val = key.get();
    auto type = val.getType();
    if (auto ptr = llvm::dyn_cast<llvm::PointerType>(type)) {
      type = ptr->getElementType();
      if (llvm::isa<llvm::Argument>(val)) {
        auto outlive = factory.getOutlive();
        alloc(key, outlive);
        if (type->isPointerTy()) {
          heap_.init(outlive).insert(outlive);
        }
      } else {
        auto local = factory.getLocal();
        alloc(key, local);
      }
    }
  }
  void Environment::alloc(const Register &key, const Location &loc) {
    stack_.init(key).insert(loc);
    heap_.init(loc);
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
