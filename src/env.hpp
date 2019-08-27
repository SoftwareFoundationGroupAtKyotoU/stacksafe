#ifndef INCLUDE_GUARD_A0BA2711_AA71_4105_83AF_E6AF119E4855
#define INCLUDE_GUARD_A0BA2711_AA71_4105_83AF_E6AF119E4855

#include <map>
#include <set>
#include "symbol.hpp"

namespace stacksafe {

template <typename Key>
class Env : private std::map<Key, std::set<Symbol>> {
  using Val = std::set<Symbol>;
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
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_A0BA2711_AA71_4105_83AF_E6AF119E4855
