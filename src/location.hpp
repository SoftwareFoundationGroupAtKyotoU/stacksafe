#ifndef INCLUDE_GUARD_6450F75E_2095_455A_B046_B7F5B19051DF
#define INCLUDE_GUARD_6450F75E_2095_455A_B046_B7F5B19051DF

#include <functional>

namespace llvm {
  class raw_ostream;
}
namespace stacksafe {
  class Location {
    static const int Global, Outlive, Local;
    friend class LocationFactory;
    const int loc_;
    explicit Location(int loc);
  public:
    std::size_t hash() const;
    void print(llvm::raw_ostream &O) const;
  };
  bool operator==(const Location &lhs, const Location &rhs);

  class LocationFactory {
    int current_;
  public:
    LocationFactory();
    static Location getGlobal();
    static Location getOutlive();
    Location getLocal();
  };
}
namespace std {
  template <>
  struct hash<stacksafe::Location> {
    size_t operator()(stacksafe::Location l) const;
  };
}

#endif//INCLUDE_GUARD_6450F75E_2095_455A_B046_B7F5B19051DF
