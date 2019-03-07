#ifndef INCLUDE_GUARD_6450F75E_2095_455A_B046_B7F5B19051DF
#define INCLUDE_GUARD_6450F75E_2095_455A_B046_B7F5B19051DF

#include <functional>

namespace llvm {
  class raw_ostream;
}
namespace stacksafe {
  class Location {
    enum class Kind : std::size_t
      { Global, Outlive, Local };
    friend class LocationFactory;
    const Kind kind_;
    const std::size_t loc_;
    explicit Location(Kind k, std::size_t n);
  public:
    size_t hash() const;
    bool operator<(const Location &rhs) const;
    bool operator==(const Location &rhs) const;
    void print(llvm::raw_ostream &O) const;
  };
  std::string to_ascii(std::size_t n);

  class LocationFactory {
    std::size_t current_;
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
