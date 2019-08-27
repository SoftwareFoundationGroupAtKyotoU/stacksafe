#ifndef INCLUDE_GUARD_A0BA2711_AA71_4105_83AF_E6AF119E4855
#define INCLUDE_GUARD_A0BA2711_AA71_4105_83AF_E6AF119E4855

#include <map>
#include "set.hpp"

namespace stacksafe {

template <typename Key>
class Env : private std::map<Key, Domain> {
  using Val = Domain;
  using Base = std::map<Key, Val>;

 public:
  using Base::begin, Base::end;
  Val* get(const Key& key) {
    const auto& self = *this;
    return const_cast<Val*>(self.get(key));
  }
  const Val* get(const Key& key) const {
    if (auto it = Base::find(key); it != end()) {
      return &it->second;
    }
    return nullptr;
  }
  void insert(const Key& key, const Symbol& sym) {
    Val val;
    val.insert(sym);
    insert(key, val);
  }
  void insert(const Key& key, const Val& val) {
    if (auto p = get(key)) {
      p->insert(val);
    } else {
      Base::try_emplace(key, val);
    }
  }
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_A0BA2711_AA71_4105_83AF_E6AF119E4855
