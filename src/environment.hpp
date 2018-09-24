#ifndef INCLUDE_GUARD_52F60E3D_17C8_4A02_BEB6_64AF9019A2B4
#define INCLUDE_GUARD_52F60E3D_17C8_4A02_BEB6_64AF9019A2B4

#include <functional>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace stacksafe {
  class Location;
}
namespace std {
  template <>
  struct hash<stacksafe::Location> {
    size_t operator()(stacksafe::Location l) const;
  };
}
namespace llvm {
  class Value;
}
namespace stacksafe {
  class Location : private std::hash<const void *> {
    using Base = std::hash<const void *>;
    friend class LocationFactory;
    const void *const loc_;
    explicit Location(const char &c);
  public:
    size_t hash() const;
    bool operator<(const Location &rhs) const;
  };
  class LocationFactory {
    std::vector<std::unique_ptr<char>> local_;
    static char outlive_, global_, undefined_;
  public:
    Location getLocal();
    Location getOutlive();
    Location getGlobal();
    Location getUndefined();
  };
  class AbstractType {
    std::unordered_set<Location> set_;
  public:
    bool subsetof(const AbstractType &rhs) const;
  };
  class HeapMap {
    std::unordered_map<Location, AbstractType> map_;
  };
  class EnvMap {
    std::unordered_map<llvm::Value *, AbstractType> map_;
  };
}

#endif//INCLUDE_GUARD_52F60E3D_17C8_4A02_BEB6_64AF9019A2B4