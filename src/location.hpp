#ifndef INCLUDE_GUARD_6450F75E_2095_455A_B046_B7F5B19051DF
#define INCLUDE_GUARD_6450F75E_2095_455A_B046_B7F5B19051DF

#include <functional>

namespace llvm {
  class raw_ostream;
}
namespace stacksafe {
  class Location {
    enum class Kind : std::size_t
      { Undef, Global, Outlive, Local };
    friend class LocationFactory;
    std::size_t loc_;
    explicit Location(Kind k);
    Location &operator++();
    Location operator++(int);
  public:
    size_t hash() const;
    bool operator<(const Location &rhs) const;
    bool operator==(const Location &rhs) const;
    void print(llvm::raw_ostream &O) const;
  };

  class LocationFactory {
    Location current_;
  public:
    LocationFactory();
    Location getUndef();
    Location getGlobal();
    Location getOutlive();
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
