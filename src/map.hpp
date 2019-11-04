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

class Map : private std::map<Number, Domain> {
  using Super = std::map<Number, Domain>;

 public:
  using Super::begin, Super::end;
  void insert(const Number &key, const Domain &val);
  void insert(const Symbol &key, const Domain &val);
  void insert(const Register &key, const Domain &val);
  Domain lookup(const Number &key) const;
  Domain lookup(const Symbol &key) const;
  Domain lookup(const Register &key) const;
  void merge(const Map &that);
  bool includes(const Map &that) const;
  Fabric diff(const Map &that) const;
};
void to_json(Json &j, const Map &x);
Fabric dump(const Map &map);

class Heap : private Map {
  using Super = Map;

 public:
  using Super::begin, Super::end, Super::includes, Super::diff;
  void insert(const Symbol &key, const Domain &val);
  Domain lookup(const Symbol &key) const;
  void merge(const Heap &that);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_A0BA2711_AA71_4105_83AF_E6AF119E4855
