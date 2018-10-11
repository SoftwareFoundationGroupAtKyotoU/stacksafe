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
}
