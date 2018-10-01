#include "environment.hpp"
#include <llvm/IR/Value.h>

namespace std {
  size_t hash<stacksafe::Location>::operator()(stacksafe::Location l) const {
    return l.hash();
  }
}
namespace stacksafe {
  Location::Location(Kind k)
    : loc_(static_cast<std::size_t>(k))
  {}
  Location &Location::operator++() {
    ++loc_;
    return *this;
  }
  Location Location::operator++(int) {
    auto ret = Location{*this};
    this->operator++();
    return ret;
  }
  size_t Location::hash() const {
    return std::hash<std::size_t>{}(loc_);
  }
  bool Location::operator<(const Location &rhs) const {
    return loc_ < rhs.loc_;
  }
  bool Location::operator==(const Location &rhs) const {
    return loc_ == rhs.loc_;
  }
  void Location::print(llvm::raw_ostream &O) const {
    O << angles(manip_single(loc_));
  }

  LocationFactory::LocationFactory()
    : current_(Location::Kind::Local) {}
  Location LocationFactory::getUndef() {
    return Location(Location::Kind::Undef);
  }
  Location LocationFactory::getGlobal() {
    return Location(Location::Kind::Global);
  }
  Location LocationFactory::getOutlive() {
    return Location(Location::Kind::Outlive);
  }
  Location LocationFactory::getLocal() {
    return current_++;
  }

  bool LocationSet::subsetof(const LocationSet &rhs) const {
    using std::begin;
    using std::end;
    return std::includes(begin(rhs), end(rhs), begin(*this), end(*this));
  }
  void LocationSet::print(llvm::raw_ostream &O) const {
    O << set_like(manip_multi(*this));
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
