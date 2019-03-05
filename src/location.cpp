#include "location.hpp"
#include "visualize.hpp"
#include <llvm/Support/raw_ostream.h>

namespace stacksafe {
  Location::Location(Kind k, std::size_t n)
    : kind_(k), loc_(n)
  {}
  size_t Location::hash() const {
    return std::hash<std::size_t>{}(static_cast<std::size_t>(kind_) + loc_);
  }
  bool Location::operator<(const Location &rhs) const {
    return loc_ < rhs.loc_;
  }
  bool Location::operator==(const Location &rhs) const {
    return loc_ == rhs.loc_;
  }
  void Location::print(llvm::raw_ostream &O) const {
    std::string id;
    switch (kind_) {
    case Kind::Global:
      id = "#_g";
      break;
    case Kind::Outlive:
      id = "#_o";
      break;
    case Kind::Local:
      id = to_ascii(loc_);
      break;
    }
    O << angles(make_manip(id));
  }
  std::string to_ascii(std::size_t n) {
    const std::size_t base = 26;
    std::string s;
    ++n;
    while (0 < n) {
      const auto digit = n % base;
      s.insert(0, 1, 'a' + digit - 1);
      n = (n - digit) / base;
    }
    return s;
  }

  LocationFactory::LocationFactory()
    : current_(0) {}
  Location LocationFactory::getGlobal() {
    return Location(Location::Kind::Global, 0);
  }
  Location LocationFactory::getOutlive() {
    return Location(Location::Kind::Outlive, 0);
  }
  Location LocationFactory::getLocal() {
    return Location(Location::Kind::Local, current_++);
  }
}
namespace std {
  size_t hash<stacksafe::Location>::operator()(stacksafe::Location l) const {
    return l.hash();
  }
}
