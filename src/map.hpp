#ifndef INCLUDE_GUARD_A0BA2711_AA71_4105_83AF_E6AF119E4855
#define INCLUDE_GUARD_A0BA2711_AA71_4105_83AF_E6AF119E4855

#include <map>
#include "domain.hpp"

namespace stacksafe {
class Register;
class Symbol;

template <typename Key>
class Map : private std::map<Key, Domain> {
  using Val = Domain;
  using Base = std::map<Key, Val>;

 public:
  using Base::begin, Base::end;
  Val* get(const Key& key);
  const Val* get(const Key& key) const;
  void insert(const Key& key, const Symbol& sym);
  void insert(const Key& key, const Val& val);
};
template <typename Key>
void to_json(Json& j, const Map<Key>& x) {
  Json obj;
  for (auto& [key, val] : x) {
    Json tmp = key;
    if (tmp.is_string()) {
      auto k = tmp.get<std::string>();
      obj[k] = val;
    }
  }
  j = obj;
}

using Stack = Map<Register>;
using Heap = Map<Symbol>;

extern template class Map<Register>;
extern template class Map<Symbol>;

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_A0BA2711_AA71_4105_83AF_E6AF119E4855
