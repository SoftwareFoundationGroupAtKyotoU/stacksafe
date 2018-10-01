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
  class Location {
    enum class Kind : std::size_t {
      Undef, Global, Outlive, Local,
    };
    friend class LocationFactory;
    std::size_t loc_;
    explicit Location(std::size_t x);
  public:
    size_t hash() const;
    bool operator<(const Location &rhs) const;
    bool operator==(const Location &rhs) const;
  };

  class LocationFactory {
    std::size_t current_;
    static const std::size_t undefined_ = 0;
    static const std::size_t global_ = 1;
    static const std::size_t outlive_ = 2;
    static const std::size_t local_ = 3;
  public:
    LocationFactory();
    Location getLocal();
    Location getOutlive();
    Location getGlobal();
    Location getUndefined();
  };

  class LocationSet : public std::unordered_set<Location> {
  public:
    bool subsetof(const LocationSet &rhs) const;
  };

  template <class Key>
  class LocationMap {
    std::unordered_map<Key, LocationSet> map_;
  public:
    bool subsetof(const LocationMap &rhs) const {
      for (auto &e : map_) {
        auto k = std::get<0>(e);
        auto &l = std::get<1>(e);
        auto it = map_.find(k);
        if (it != end(map_)) {
          if (l.subsetof(std::get<1>(*it))) {
            continue;
          }
        }
        return false;
      }
      return true;
    }
    void unify(const LocationMap &rhs) {
      using std::begin;
      using std::end;
      for (auto &e : rhs.map_) {
        auto k = std::get<0>(e);
        auto &r = std::get<1>(e);
        if (map_.count(k) == 0) {
          map_.insert(e);
        } else {
          map_.at(k).insert(begin(r), end(r));
        }
      }
    }
  };

  class Environment {
    LocationMap<Location> heap_;
    LocationMap<llvm::Value *> stack_;
  public:
    bool subsetof(const Environment &rhs) const;
    void unify(const Environment &rhs);
  };
}

#endif//INCLUDE_GUARD_52F60E3D_17C8_4A02_BEB6_64AF9019A2B4
