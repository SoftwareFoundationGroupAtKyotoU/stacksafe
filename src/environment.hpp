#ifndef INCLUDE_GUARD_52F60E3D_17C8_4A02_BEB6_64AF9019A2B4
#define INCLUDE_GUARD_52F60E3D_17C8_4A02_BEB6_64AF9019A2B4

#include "location.hpp"
#include "register.hpp"
#include "visualize.hpp"
#include <functional>
#include <memory>
#include <unordered_map>
#include <unordered_set>

namespace llvm {
  class raw_ostream;
}
namespace stacksafe {
  class LocationSet : public std::unordered_set<Location> {
  public:
    bool subsetof(const LocationSet &rhs) const;
    void unify(const LocationSet &rhs);
    void print(llvm::raw_ostream &O) const;
  };

  template <class Key>
  class LocationMap : public std::unordered_map<Key, LocationSet> {
    template <typename T>
    using OptRef = std::optional<std::reference_wrapper<T>>;
  public:
    OptRef<const LocationSet> get(const Key &k) const {
      auto it = this->find(k);
      if (it != end(*this)) {
        return std::get<1>(*it);
      }
      return std::nullopt;
    }
    OptRef<LocationSet> get(const Key &k) {
      auto it = this->find(k);
      if (it != end(*this)) {
        return std::get<1>(*it);
      }
      return std::nullopt;
    }
    bool subsetof(const LocationMap &rhs) const {
      auto f = [&rhs](const auto &e) -> bool {
        auto k = std::get<0>(e);
        auto &l = std::get<1>(e);
        if (auto r = rhs.get(k)) {
          return l.subsetof(r->get());
        } else {
          return false;
        }
      };
      return std::all_of(begin(*this), end(*this), f);
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
      O << set_like(foreach(key_value, *this));
    }
  };

  class Environment {
    LocationMap<Location> heap_;
    LocationMap<Register> stack_;
  public:
    bool subsetof(const Environment &rhs) const;
    void unify(const Environment &rhs);
    void print(llvm::raw_ostream &O) const;
  };
}

#endif//INCLUDE_GUARD_52F60E3D_17C8_4A02_BEB6_64AF9019A2B4
