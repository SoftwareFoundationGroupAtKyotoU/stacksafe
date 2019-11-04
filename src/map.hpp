#ifndef INCLUDE_GUARD_A0BA2711_AA71_4105_83AF_E6AF119E4855
#define INCLUDE_GUARD_A0BA2711_AA71_4105_83AF_E6AF119E4855

#include <map>
#include <optional>
#include "domain.hpp"
#include "json_fwd.hpp"

namespace stacksafe {
class Fabric;
class Register;
class Symbol;

template <typename K>
class Map : private std::map<K, Domain> {
  using Super = std::map<K, Domain>;
  std::optional<Domain> get(const K &key) const;
  bool insert(const Number &key, const Domain &val);

 public:
  using Super::begin, Super::end;
  bool insert(const K &key, const Domain &val);
  bool merge(const Map &that);
  bool includes(const Map &that) const;
  Domain lookup(const K &key) const;
  Fabric diff(const Map &that) const;
};
template <typename K>
void to_json(Json &j, const Map<K> &x);
template <typename K>
Fabric dump(const Map<K> &map);

extern template class Map<Register>;
extern template class Map<Symbol>;
extern template void to_json<Register>(Json &, const Map<Register> &);
extern template void to_json<Symbol>(Json &, const Map<Symbol> &);
extern template Fabric dump<Register>(const Map<Register> &);
extern template Fabric dump<Symbol>(const Map<Symbol> &);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_A0BA2711_AA71_4105_83AF_E6AF119E4855
