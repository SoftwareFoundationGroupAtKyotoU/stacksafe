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

  class LocationSet {
    std::unordered_set<Location> set_;
  public:
    bool subsetof(const LocationSet &rhs) const;
  };

  template <class Key>
  class LocationMap {
    using Maybe = std::optional<std::reference_wrapper<const LocationSet>>;
    std::unordered_map<Key, LocationSet> map_;
  public:
    bool subsetof(const LocationMap &rhs) const {
      for (auto &e : map_) {
        auto k = std::get<0>(e);
        auto &l = std::get<1>(e);
        if (auto r = rhs.get(k)) {
          if (l.subsetof(*r)) {
            continue;
          }
        }
        return false;
      }
      return true;
    }
  private:
    Maybe get(Key key) const {
      auto it = map_.find(key);
      if (it == end(map_)) {
        return std::nullopt;
      } else {
        return std::cref(std::get<1>(*it));
      }
    }
  };
}

#endif//INCLUDE_GUARD_52F60E3D_17C8_4A02_BEB6_64AF9019A2B4
