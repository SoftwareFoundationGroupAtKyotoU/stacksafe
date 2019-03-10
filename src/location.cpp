#include "location.hpp"
#include "visualize.hpp"
#include <llvm/Support/raw_ostream.h>

namespace stacksafe {
  const std::size_t Location::Global = static_cast<std::size_t>(-1);
  const std::size_t Location::Outlive = 0;
  const std::size_t Location::Local = 1;
  Location::Location(std::size_t loc)
    : loc_(loc)
  {}
  size_t Location::hash() const {
    return std::hash<std::size_t>{}(loc_);
  }
  void Location::print(llvm::raw_ostream &O) const {
    auto ascii =
      [](std::size_t n) -> std::string {
        switch (n) {
        case Location::Global:
          return "#_g";
        case Location::Outlive:
          return "#_o";
        }
        const std::size_t base = 26;
        std::string s;
        while (0 < n) {
          const auto digit = (n - 1) % base;
          s.append(1, 'a' + digit);
          n = (n - digit - 1) / base;
        }
        s.append("#");
        std::reverse(begin(s), end(s));
        return s;
      };
    O << angles(make_manip(ascii(loc_)));
  }
  bool operator==(const Location &lhs, const Location &rhs) {
    return lhs.hash() == rhs.hash();
  }

  LocationFactory::LocationFactory()
    : current_(Location::Local) {}
  Location LocationFactory::getGlobal() {
    return Location(Location::Global);
  }
  Location LocationFactory::getOutlive() {
    return Location(Location::Outlive);
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
