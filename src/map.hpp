#ifndef INCLUDE_GUARD_A0BA2711_AA71_4105_83AF_E6AF119E4855
#define INCLUDE_GUARD_A0BA2711_AA71_4105_83AF_E6AF119E4855

#include <map>
#include "domain.hpp"
#include "json_fwd.hpp"
#include "register.hpp"
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
  void insert(const K& key, const Domain& val);
  void insert(const Map& that);
  bool includes(const Map& that) const;
  Domain lookup(const K& key) const;
};
template <typename K>
void to_json(Json& j, const Map<K>& x);
template <typename K>
Fabric dump(const Map<K>& map);

using Heap = Map<Symbol>;
using RegMap = Map<Register>;

extern template class Map<Symbol>;
extern template class Map<Register>;

extern template void to_json<Symbol>(Json&, const Heap&);
extern template void to_json<Register>(Json&, const Map<Register>&);

extern template Fabric dump<Symbol>(const Heap&);
extern template Fabric dump<Register>(const Map<Register>&);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_A0BA2711_AA71_4105_83AF_E6AF119E4855
