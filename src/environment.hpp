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

  template <typename T>
  using OptRef = std::optional<std::reference_wrapper<T>>;
  template <class Key>
  class LocationMap : public std::unordered_map<Key, LocationSet> {
    OptRef<const LocationSet> get(const Key &k) const {
      if (auto it = this->find(k); it != end(*this)) {
        return std::get<1>(*it);
      } else {
        return std::nullopt;
      }
    }
  public:
    bool exists(const Key &k) const {
      return this->count(k) != 0;
    }
    OptRef<LocationSet> get(const Key &k) {
      if (auto it = this->find(k); it != end(*this)) {
        return std::get<1>(*it);
      } else {
        llvm::errs() << "Error: " << spaces(make_manip(k)) << "is not in a map" << endl;
        return std::nullopt;
      }
    }
    OptRef<LocationSet> init(const Key &k) {
      if (exists(k)) {
        llvm::errs() << "Error: " << spaces(make_manip(k)) << "is in a map" << endl;
        return std::nullopt;
      } else {
        auto [it, _] = this->emplace(k, LocationSet{});
        return std::get<1>(*it);
      }
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
      auto f = [&lhs = *this](const auto &e) {
        auto k = std::get<0>(e);
        auto &r = std::get<1>(e);
        if (auto l = lhs.get(k)) {
          l->get().unify(r);
        } else {
          lhs.insert(e);
        }
      };
      std::for_each(begin(rhs), end(rhs), f);
    }
    void print(llvm::raw_ostream &O) const {
      O << set_like(foreach(key_value, *this));
    }
  };

  class Environment {
    LocationMap<Location> heap_;
    LocationMap<Register> stack_;
    LocationFactory &factory_;
  public:
    explicit Environment(LocationFactory &factory);
    bool subsetof(const Environment &rhs) const;
    void unify(const Environment &rhs);
    void print(llvm::raw_ostream &O) const;
    LocationSet &init(const Location &key);
    LocationSet &init(const Register &key);
    bool initArg(const Register &key);
    bool alloc(const Register &key);
    bool load(const Register &dst, const Register &src);
    OptRef<LocationSet> get(const Location &key);
    OptRef<LocationSet> get(const Register &key);
  };
}

#endif//INCLUDE_GUARD_52F60E3D_17C8_4A02_BEB6_64AF9019A2B4
