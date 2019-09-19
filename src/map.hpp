#ifndef INCLUDE_GUARD_A0BA2711_AA71_4105_83AF_E6AF119E4855
#define INCLUDE_GUARD_A0BA2711_AA71_4105_83AF_E6AF119E4855

#include <map>
#include "domain.hpp"
#include "json_fwd.hpp"
#include "symbol.hpp"
#include "value.hpp"

namespace stacksafe {
class Fabric;

template <typename K>
class Map : private std::map<K, Domain> {
  using Super = std::map<K, Domain>;

 public:
  using Super::begin, Super::end;
  bool exists(const K& key) const;
  Domain* get(const K& key);
  const Domain* get(const K& key) const;
  void insert(const K& key, const Symbol& sym);
  void insert(const K& key, const Domain& val);
  void insert(const K& key);
  void insert(const Map& that);
  bool includes(const Map& that) const;
};
template <typename K>
void to_json(Json& j, const Map<K>& x);
template <typename K>
Fabric dump(const Map<K>& map);

using Stack = Map<Value>;
using Heap = Map<Symbol>;

extern template class Map<Value>;
extern template class Map<Symbol>;

extern template void to_json<Value>(Json&, const Stack&);
extern template void to_json<Symbol>(Json&, const Heap&);

extern template Fabric dump<Value>(const Stack&);
extern template Fabric dump<Symbol>(const Heap&);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_A0BA2711_AA71_4105_83AF_E6AF119E4855
