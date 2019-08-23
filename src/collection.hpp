#ifndef INCLUDE_GUARD_6495011D_0D6C_4DD2_8111_D661A86391FD
#define INCLUDE_GUARD_6495011D_0D6C_4DD2_8111_D661A86391FD

#include <algorithm>
#include <functional>
#include <initializer_list>
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include "visualize.hpp"

namespace llvm {
class raw_ostream;
}  // namespace llvm

namespace stacksafe {
template <typename T>
class OptRef : private std::optional<std::reference_wrapper<T>> {
  using Base = std::optional<std::reference_wrapper<T>>;

 public:
  using Base::operator bool;
  OptRef() {}
  OptRef(std::nullopt_t v) : Base{v} {}
  OptRef(T &v) : Base{v} {}
  T *operator->() { return &value(); }
  T &operator*() & { return value(); }
  T &&operator*() && { return value(); }
  T &value() & { return Base::value().get(); }
  T &&value() && { return Base::value().get(); }
  const T *operator->() const { return &value(); }
  const T &operator*() const & { return value(); }
  const T &&operator*() const && { return value(); }
  const T &value() const & { return Base::value().get(); }
  const T &&value() const && { return Base::value().get(); }
};

template <typename T>
class Set : private std::vector<T> {
  using Base = std::vector<T>;
  using iterator = typename Base::iterator;

 public:
  using Base::begin, Base::end;
  Set(std::initializer_list<T> init) : Base{init} { std::sort(begin(), end()); }
  void insert(const T &v) { insert(begin(), v); }
  void insert(const Set &that) {
    auto it = begin();
    for (auto &v : that) {
      it = std::next(insert(it, v));
    }
  }
  bool exists(const T &v) const {
    return std::binary_search(begin(), end(), v);
  }
  bool subsetof(const Set &that) const {
    return std::includes(that.begin(), that.end(), begin(), end());
  }
  void print(llvm::raw_ostream &O) const { O << set_like(*this); }

 protected:
  iterator insert(iterator begin, const T &v) {
    auto [lb, ub] = std::equal_range(begin, end(), v);
    if (lb == ub) {
      return Base::insert(lb, v);
    }
    return lb;
  }
};

template <typename K, typename T>
class MapValue : private std::tuple<K, T> {
  using Base = std::tuple<K, T>;

 public:
  MapValue(const K &k, const T &t) : Base{k, t} {}
  const K &key() const { return std::get<0>(*this); }
  const T &value() const { return std::get<1>(*this); }
  T &value() { return std::get<1>(*this); }
  bool operator<(const MapValue &that) const { return key() < that.key(); }
};
template <typename K, typename T>
class Map : private Set<MapValue<K, T>> {
  using V = MapValue<K, T>;
  using Base = Set<V>;

 public:
  using Base::begin, Base::end;
  Map() : Base{} {}
  const T *get(const K &k) const {
    auto [lb, ub] = std::equal_range(begin(), end(), V{k, T{}});
    if (lb == ub) {
      return nullptr;
    } else {
      return &lb->value();
    }
  }
  T *get(const K &k) {
    return const_cast<T *>(static_cast<const Map &>(*this).get(k));
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

template <typename K, typename T>
class Env : private Map<K, Set<T>> {
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
  void insert(const Env &that) {
    for (auto &v : that) {
      insert(v.key(), v.value());
    }
  }
  bool subsetof(const Env &that) const {
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
};
}  // namespace stacksafe

#endif  // INCLUDE_GUARD_6495011D_0D6C_4DD2_8111_D661A86391FD
