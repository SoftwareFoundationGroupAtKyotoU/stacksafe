#ifndef INCLUDE_GUARD_A0BA2711_AA71_4105_83AF_E6AF119E4855
#define INCLUDE_GUARD_A0BA2711_AA71_4105_83AF_E6AF119E4855

#include <functional>
#include <unordered_map>
#include "domain.hpp"
#include "value.hpp"

namespace stacksafe {

class Map : private std::unordered_map<Value, Domain> {
  using Super = std::unordered_map<Value, Domain>;

 public:
  using Super::begin, Super::end;
  void insert(const Value &key, const Domain &val);
  Domain lookup(const Value &key) const;
  void merge(const Map &that);
  bool includes(const Map &that) const;
  static std::size_t hash(const Map &m);
};

}  // namespace stacksafe

namespace std {
template <>
struct hash<stacksafe::Map> {
  size_t operator()(const stacksafe::Map &m) const;
};
}  // namespace std

#endif  // INCLUDE_GUARD_A0BA2711_AA71_4105_83AF_E6AF119E4855
