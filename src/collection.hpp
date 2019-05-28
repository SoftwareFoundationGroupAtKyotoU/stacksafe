#ifndef INCLUDE_GUARD_6495011D_0D6C_4DD2_8111_D661A86391FD
#define INCLUDE_GUARD_6495011D_0D6C_4DD2_8111_D661A86391FD

#include "visualize.hpp"
#include <algorithm>
#include <functional>
#include <optional>
#include <unordered_map>
#include <unordered_set>

namespace llvm {
class raw_ostream;
} // namespace llvm

namespace stacksafe {
template <typename T>
class OptRef : private std::optional<std::reference_wrapper<T>> {
  using Base = std::optional<std::reference_wrapper<T>>;

public:
  using Base::operator bool;
  OptRef();
  OptRef(std::nullopt_t v) : Base{v} {}
  OptRef(T &v) : Base{v} {}
  const T *operator->() const { return &value(); }
  const T &operator*() const & { return value(); }
  const T &&operator*() const && { return value(); }
  const T &value() const & { return Base::value().get(); }
  const T &&value() const && { return Base::value().get(); }
  T *operator->() { return &value(); }
  T &operator*() & { return value(); }
  T &&operator*() && { return value(); }
  T &value() & { return Base::value().get(); }
  T &&value() && { return Base::value().get(); }
};

template <typename T> class Set : private std::unordered_set<T> {
  using Base = std::unordered_set<T>;

public:
  using Base::Base, Base::begin, Base::end, Base::size, Base::insert;
  bool subsetof(const Set &rhs) const {
    auto pred = [&rhs](const T &t) { return rhs.exists(t); };
    return std::all_of(begin(), end(), std::move(pred));
  }
  void unify(const Set &rhs) { insert(rhs.begin(), rhs.end()); }
  bool exists(const T &t) const { return Base::count(t) != 0; }
  void print(llvm::raw_ostream &O) const {
    O << set_like(make_manip_seq(*this));
  }
};

template <typename K, typename T>
class Map : private std::unordered_map<K, Set<T>> {
  using Base = std::unordered_map<K, Set<T>>;
  using V = Set<T>;

public:
  using Base::Base, Base::begin, Base::end, Base::size;
  OptRef<const V> get(const K &k) const {
    if (auto it = Base::find(k); it != end()) {
      return std::get<1>(*it);
    } else {
      return std::nullopt;
    }
  }
  OptRef<V> get(const K &k) {
    if (auto it = Base::find(k); it != end()) {
      return std::get<1>(*it);
    } else {
      return std::nullopt;
    }
  }
  bool add(const K &k) {
    if (exists(k)) {
      return true;
    } else {
      Base::emplace(k, V{});
      return false;
    }
  }
  bool add(const K &k, const T &t) {
    if (auto e = get(k)) {
      e->insert(t);
      return true;
    } else {
      Base::emplace(k, V{{t}});
      return false;
    }
  }
  bool add(const K &k, const V &v) {
    if (auto e = get(k)) {
      e->unify(v);
      return true;
    } else {
      Base::emplace(k, v);
      return false;
    }
  }
  bool exists(const K &k) const { return Base::count(k) != 0; }
  bool exists(const Set<K> &ks) const {
    auto pred = [&self = *this](const K &k) { return self.exists(k); };
    return std::all_of(ks.begin(), ks.end(), std::move(pred));
  }
  bool subsetof(const Map &rhs) const {
    auto f = [&rhs](const auto &e) {
      auto &[k, l] = e;
      if (auto r = rhs.get(k)) {
        return l.subsetof(*r);
      } else {
        return false;
      }
    };
    return std::all_of(begin(), end(), std::move(f));
  }
  void unify(const Map &rhs) {
    auto f = [&lhs = *this](const auto &e) {
      auto &[k, r] = e;
      if (auto l = lhs.get(k)) {
        l->unify(r);
      } else {
        lhs.insert(e);
      }
    };
    std::for_each(rhs.begin(), rhs.end(), std::move(f));
  }
  void print(llvm::raw_ostream &O) const {
    O << set_like(foreach (key_value, *this));
  }
};
} // namespace stacksafe

#endif // INCLUDE_GUARD_6495011D_0D6C_4DD2_8111_D661A86391FD
