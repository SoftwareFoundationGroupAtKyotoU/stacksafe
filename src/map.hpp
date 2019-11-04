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
class Map : private std::map<Number, Domain> {
  using Super = std::map<Number, Domain>;
  bool insert(const Number &key, const Domain &val);
  Domain lookup(const Number &key) const;

 public:
  using Super::begin, Super::end;
  bool insert(const Symbol &key, const Domain &val);
  bool insert(const Register &key, const Domain &val);
  Domain lookup(const Symbol &key) const;
  Domain lookup(const Register &key) const;
  bool merge(const Map &that);
  bool includes(const Map &that) const;
  Fabric diff(const Map &that) const;
};
void to_json(Json &j, const Map<Symbol> &x);
void to_json(Json &j, const Map<Register> &x);
template <typename K>
Fabric dump(const Map<K> &map);

extern template class Map<Register>;
extern template class Map<Symbol>;
extern template Fabric dump<Register>(const Map<Register> &);
extern template Fabric dump<Symbol>(const Map<Symbol> &);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_A0BA2711_AA71_4105_83AF_E6AF119E4855
