#include "environment.hpp"
#include <llvm/IR/Value.h>

namespace std {
  size_t hash<stacksafe::Location>::operator()(stacksafe::Location l) const {
    return l.hash();
  }
}
namespace stacksafe {
  Location::Location(std::size_t x)
    : loc_(x)
  {}
  size_t Location::hash() const {
    return std::hash<std::size_t>{}(loc_);
  }
  bool Location::operator<(const Location &rhs) const {
    return loc_ < rhs.loc_;
  }
  bool Location::operator==(const Location &rhs) const {
    return loc_ == rhs.loc_;
  }

  LocationFactory::LocationFactory()
    : current_(local_) {}
  Location LocationFactory::getLocal() {
    return Location(current_++);
  }
  Location LocationFactory::getOutlive() {
    return Location(outlive_);
  }
  Location LocationFactory::getGlobal() {
    return Location(global_);
  }
  Location LocationFactory::getUndefined() {
    return Location(undefined_);
  }

  bool LocationSet::subsetof(const LocationSet &rhs) const {
    using std::begin;
    using std::end;
    return std::includes(begin(rhs), end(rhs), begin(*this), end(*this));
  }

  bool Environment::subsetof(const Environment &rhs) const {
    return heap_.subsetof(rhs.heap_) && stack_.subsetof(rhs.stack_);
  }
  void Environment::unify(const Environment &rhs) {
    heap_.unify(rhs.heap_);
    stack_.unify(rhs.stack_);
  }
}
