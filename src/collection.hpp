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

template <typename T> class Set : private std::vector<T> {
  using Base = std::vector<T>;
  using iterator = typename Base::iterator;

public:
  using Base::begin, Base::end;
  Set(std::initializer_list<T> init) : Base{init} { std::sort(begin(), end()); }
  void insert(const T &v) { iterative_insert(begin(), v); }
  void insert(const Set &that) {
    auto it = begin();
    for (auto &v : that) {
      it = std::next(iterative_insert(it, v));
    }
  }
  bool exists(const T &v) const {
    return std::binary_search(begin(), end(), v);
  }
  bool subsetof(const Set &that) const {
    return std::includes(that.begin(), that.end(), begin(), end());
  }
  void print(llvm::raw_ostream &O) const { O << set_like(*this); }

private:
  iterator iterative_insert(iterator begin, const T &v) {
    auto [lb, ub] = std::equal_range(begin, end(), v);
    if (lb == ub) {
      return Base::insert(lb, v);
    }
    return lb;
  }
};

template <typename K, typename T> class MapValue : private std::tuple<K, T> {
  using Base = std::tuple<K, T>;

public:
  MapValue(const K &k, const T &t) : Base{k, t} {}
  const K &key() const { return std::get<0>(*this); }
  const T &value() const { return std::get<1>(*this); }
  T &value() { return std::get<1>(*this); }
  bool operator<(const MapValue &that) const { return key() < that.key(); }
};
template <typename K, typename T> class Map : private Set<MapValue<K, T>> {
  using V = MapValue<K, T>;
  using Base = Set<V>;

public:
  using Base::begin, Base::end;
  Map() : Base{} {}
  OptRef<const T> get(const K &k) const {
    auto [lb, ub] = std::equal_range(begin(), end(), V{k, T{}});
    if (lb == ub) {
      return std::nullopt;
    } else {
      return lb->value();
    }
  }
  OptRef<T> get(const K &k) {
    if (auto ref = static_cast<const Map &>(*this).get(k)) {
      return const_cast<T &>(*ref);
    } else {
      return std::nullopt;
    }
  }
  bool exists(const K &k) const { return Base::exists(V{k, T{}}); }
  void print(llvm::raw_ostream &O) const {
    auto key_value = [](const V &v) { return manip(v.key(), v.value()); };
    O << set_like(for_each(key_value, *this));
  }

protected:
  using value_type = V;
  using Base::insert;
};

template <typename K, typename T> class MapSet : private Map<K, Set<T>> {
  using V = Set<T>;
  using Base = Map<K, V>;
  using value_type = typename Base::value_type;

public:
  using Base::begin, Base::end;
  using Base::get, Base::exists, Base::print;
  bool insert(const K &k) {
    if (exists(k)) {
      return true;
    } else {
      Base::insert(value_type{k, V{}});
      return false;
    }
  }
  bool insert(const K &k, const T &t) { return insert(k, V{t}); };
  bool insert(const K &k, const V &v) {
    if (auto ref = get(k)) {
      ref->insert(v);
      return true;
    } else {
      Base::insert(value_type{k, v});
      return false;
    }
  }
  void insert(const MapSet &that) {
    for (auto &v : that) {
      insert(v.key(), v.value());
    }
  }
  bool subsetof(const MapSet &that) const {
    for (auto &v : *this) {
      if (auto ref = that.get(v.key())) {
        if (v.value().subsetof(*ref)) {
          continue;
        }
      }
      return false;
    }
    return true;
  }

private:
};

template <typename K, typename T>
class Map_ : private std::unordered_map<K, Set<T>> {
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
  bool subsetof(const Map_ &rhs) const {
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
  void unify(const Map_ &rhs) {
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
    // O << set_like(for_each(key_value, *this));
  }
};
} // namespace stacksafe

#endif // INCLUDE_GUARD_6495011D_0D6C_4DD2_8111_D661A86391FD
