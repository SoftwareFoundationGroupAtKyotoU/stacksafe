#ifndef INCLUDE_GUARD_6495011D_0D6C_4DD2_8111_D661A86391FD
#define INCLUDE_GUARD_6495011D_0D6C_4DD2_8111_D661A86391FD

#include "visualize.hpp"
#include <algorithm>
#include <functional>
#include <initializer_list>
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

template <typename T, typename Cmp = std::less<T>>
class Set : private std::vector<T> {
  using Base = std::vector<T>;
  using iterator = typename Base::iterator;
  iterator insert(iterator begin, const T &v) {
    auto [lb, ub] = std::equal_range(begin, end(), v, Cmp{});
    if (lb == ub) {
      return Base::insert(lb, v);
    }
    return lb;
  }

public:
  using Base::begin, Base::end;
  Set(std::initializer_list<T> init) : Base{init} {
    std::sort(begin(), end(), Cmp{});
  }
  void insert(const T &v) { insert(begin(), v); }
  void insert(const Set &that) {
    auto it = begin();
    for (auto &v : that) {
      it = std::next(insert(it, v));
    }
  }
  bool exists(const T &v) const {
    return std::binary_search(begin(), end(), v, Cmp{});
  }
  bool subsetof(const Set &that) const {
    return std::includes(that.begin(), that.end(), begin(), end(), Cmp{});
  }
  void print(llvm::raw_ostream &O) const { O << set_like(*this); }
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
      e->insert(v);
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
        l->insert(r);
      } else {
        lhs.insert(e);
      }
    };
    std::for_each(rhs.begin(), rhs.end(), std::move(f));
  }
  void print(llvm::raw_ostream &O) const {
    O << set_like(for_each(key_value, *this));
  }
};
} // namespace stacksafe

#endif // INCLUDE_GUARD_6495011D_0D6C_4DD2_8111_D661A86391FD
