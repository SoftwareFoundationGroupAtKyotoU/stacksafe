#ifndef INCLUDE_GUARD_6495011D_0D6C_4DD2_8111_D661A86391FD
#define INCLUDE_GUARD_6495011D_0D6C_4DD2_8111_D661A86391FD

#include "visualize.hpp"
#include <algorithm>
#include <unordered_set>
#include <unordered_map>

namespace llvm {
  class raw_ostream;
}
namespace stacksafe {
  template <typename T>
  class Set : public std::unordered_set<T> {
  public:
    bool subsetof(const Set &rhs) const {
      return std::includes(begin(rhs), end(rhs), begin(*this), end(*this));
    }
    void unify(const Set &rhs) {
      this->insert(begin(rhs), end(rhs));
    }
    void print(llvm::raw_ostream &O) const {
      O << set_like(make_manip_seq(*this));
    }
  };

  template <typename K, typename T>
  class Map : public std::unordered_map<K, Set<T>> {
  public:
    using V = Set<T>;
    const V *get(const K &k) const {
      if (auto it = this->find(k); it != end(*this)) {
        return &std::get<1>(*it);
      } else {
        return nullptr;
      }
    }
    V *get(const K &k) {
      const auto &c = *this;
      return const_cast<V *>(c.get(k));
    }
    void add(const K &k, const T &t) {
      if (auto e = get(k)) {
        e->insert(t);
      } else {
        this->emplace(k, V{{t}});
      }
    }
    void add(const K &k, const V &v) {
      if (auto e = get(k)) {
        e->unify(v);
      } else {
        this->emplace(k, v);
      }
    }
    bool exists(const K &k) const {
      return this->count(k) != 0;
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
      return std::all_of(begin(*this), end(*this), std::move(f));
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
      std::for_each(begin(rhs), end(rhs), std::move(f));
    }
    void print(llvm::raw_ostream &O) const {
      O << set_like(foreach(key_value, *this));
    }
  };
}

#endif//INCLUDE_GUARD_6495011D_0D6C_4DD2_8111_D661A86391FD
