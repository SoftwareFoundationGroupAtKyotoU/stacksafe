#include "environment.hpp"
#include <llvm/IR/Value.h>

namespace std {
  size_t hash<stacksafe::Location>::operator()(stacksafe::Location l) const {
    return l.hash();
  }
}
namespace stacksafe {
  Location::Location(const char &c)
    : loc_(&c)
  {}
  size_t Location::hash() const {
    return Base::operator()(loc_);
  }
  bool Location::operator<(const Location &rhs) const {
    return loc_ < rhs.loc_;
  }
  bool Location::operator==(const Location &rhs) const {
    return loc_ == rhs.loc_;
  }

  namespace {
    constexpr char nullch = '\0';
  }
  char LocationFactory::outlive_ = nullch;
  char LocationFactory::global_ = nullch;
  char LocationFactory::undefined_ = nullch;
  Location LocationFactory::getLocal() {
    local_.push_back(std::make_unique<char>(nullch));
    return static_cast<Location>(*local_.back());
  }
  Location LocationFactory::getOutlive() {
    return static_cast<Location>(outlive_);
  }
  Location LocationFactory::getGlobal() {
    return static_cast<Location>(global_);
  }
  Location LocationFactory::getUndefined() {
    return static_cast<Location>(undefined_);
  }

  bool LocationSet::subsetof(const LocationSet &rhs) const {
    return std::includes(rhs.begin(), rhs.end(), begin(), end());
  }

  bool Environment::subsetof(const Environment &rhs) const {
    return heap_.subsetof(rhs.heap_) && stack_.subsetof(rhs.stack_);
  }
  void Environment::unify(const Environment &rhs) {
    heap_.unify(rhs.heap_);
    stack_.unify(rhs.stack_);
  }
}
