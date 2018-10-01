#ifndef INCLUDE_GUARD_52F60E3D_17C8_4A02_BEB6_64AF9019A2B4
#define INCLUDE_GUARD_52F60E3D_17C8_4A02_BEB6_64AF9019A2B4

#include "visualize.hpp"
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
  class raw_ostream;
  class Value;
}
namespace stacksafe {
  class Location {
    enum class Kind : std::size_t {
      Undef, Global, Outlive, Local,
    };
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

  class LocationSet : public std::unordered_set<Location> {
  public:
    bool subsetof(const LocationSet &rhs) const;
    void print(llvm::raw_ostream &O) const;
  };

  template <class Key>
  class LocationMap : public std::unordered_map<Key, LocationSet> {
  public:
    bool subsetof(const LocationMap &rhs) const {
      for (auto &e : *this) {
        auto k = std::get<0>(e);
        auto &l = std::get<1>(e);
        auto it = this->find(k);
        if (it != end(*this)) {
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
      for (auto &e : rhs) {
        auto k = std::get<0>(e);
        auto &r = std::get<1>(e);
        if (this->count(k) == 0) {
          this->insert(e);
        } else {
          this->at(k).insert(begin(r), end(r));
        }
      }
    }
    void print(llvm::raw_ostream &O) const {
      const auto kv = [](const auto &x) -> ManipObj {
        auto k = wrap(std::get<0>(x));
        auto s = wrap(": ");
        auto v = wrap(std::get<1>(x));
        return wrap(Manip{k, s, v});
      };
      O << set_like(foreach(kv, *this));
    }
  };

  class Environment {
    LocationMap<Location> heap_;
    LocationMap<llvm::Value *> stack_;
  public:
    bool subsetof(const Environment &rhs) const;
    void unify(const Environment &rhs);
    void print(llvm::raw_ostream &O) const;
  };
}

#endif//INCLUDE_GUARD_52F60E3D_17C8_4A02_BEB6_64AF9019A2B4
