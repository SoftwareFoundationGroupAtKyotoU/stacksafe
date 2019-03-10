#include "location.hpp"
#include "visualize.hpp"
#include <llvm/Support/raw_ostream.h>
#include <string>

namespace stacksafe {
  const int Location::Global = -1;
  const int Location::Outlive = 0;
  const int Location::Local = 1;
  Location::Location(int loc)
    : loc_(loc)
  {}
  std::size_t Location::hash() const {
    return std::hash<int>{}(loc_);
  }
  void Location::print(llvm::raw_ostream &O) const {
    auto ascii =
      [](int n) -> std::string {
        switch (n) {
        case Location::Global:
          return "#_g";
        case Location::Outlive:
          return "#_o";
        }
        const int base = 26;
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
