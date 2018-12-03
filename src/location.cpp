#include "location.hpp"
#include "visualize.hpp"
#include <llvm/Support/raw_ostream.h>

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
    O << angles(make_manip("#", loc_));
  }

  LocationFactory::LocationFactory()
    : current_(Location::Kind::Local) {}
  Location LocationFactory::getGlobal() {
    return Location(Location::Kind::Global);
  }
  Location LocationFactory::getOutlive() {
    return Location(Location::Kind::Outlive);
  }
  Location LocationFactory::getLocal() {
    return current_++;
  }
}
namespace std {
  size_t hash<stacksafe::Location>::operator()(stacksafe::Location l) const {
    return l.hash();
  }
}
