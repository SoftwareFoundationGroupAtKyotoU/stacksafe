#ifndef INCLUDE_GUARD_52F60E3D_17C8_4A02_BEB6_64AF9019A2B4
#define INCLUDE_GUARD_52F60E3D_17C8_4A02_BEB6_64AF9019A2B4

#include <functional>
#include <memory>
#include <optional>
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
    bool operator==(const Location &rhs) const;
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
    using Key = Location;
    using Maybe = std::optional<std::reference_wrapper<const AbstractType>>;
    std::unordered_map<Key, AbstractType> map_;
  public:
    bool subsetof(const HeapMap &rhs) const;
    Maybe get(Key key) const;
  };
  class EnvMap {
    using Key = llvm::Value *;
    using Maybe = std::optional<std::reference_wrapper<const AbstractType>>;
    std::unordered_map<Key, AbstractType> map_;
  public:
    bool subsetof(const EnvMap &rhs) const;
    Maybe get(Key key) const;
  };
}

#endif//INCLUDE_GUARD_52F60E3D_17C8_4A02_BEB6_64AF9019A2B4
