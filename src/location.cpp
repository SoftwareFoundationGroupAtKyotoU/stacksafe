#include "location.hpp"
#include "visualize.hpp"
#include <llvm/Support/raw_ostream.h>

namespace stacksafe {
  Location::Location(std::size_t loc)
    : loc_(loc)
  {}
  size_t Location::hash() const {
    return std::hash<std::size_t>{}(loc_);
  }
  void Location::print(llvm::raw_ostream &O) const {
    std::string id;
    switch (loc_) {
    case static_cast<std::size_t>(-1):
      id = "_g";
      break;
    case 0:
      id = "_o";
      break;
    default:
      id = to_ascii(loc_);
      break;
    }
    O << angles(make_manip("#", id));
  }
  std::string to_ascii(std::size_t n) {
    const std::size_t base = 26;
    std::string s;
    while (0 < n) {
      const auto digit = (n - 1) % base;
      s.insert(0, 1, 'a' + digit);
      n = (n - digit - 1) / base;
    }
    return s;
  }
  bool operator==(const Location &lhs, const Location &rhs) {
    return lhs.hash() == rhs.hash();
  }

  LocationFactory::LocationFactory()
    : current_(1) {}
  Location LocationFactory::getGlobal() {
    return Location(-1);
  }
  Location LocationFactory::getOutlive() {
    return Location(0);
  }
  Location LocationFactory::getLocal() {
    return Location(current_++);
  }
}
namespace std {
  size_t hash<stacksafe::Location>::operator()(stacksafe::Location l) const {
    return l.hash();
  }
}
