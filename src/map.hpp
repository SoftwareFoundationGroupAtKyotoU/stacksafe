#ifndef INCLUDE_GUARD_A0BA2711_AA71_4105_83AF_E6AF119E4855
#define INCLUDE_GUARD_A0BA2711_AA71_4105_83AF_E6AF119E4855

#include <functional>
#include <unordered_map>
#include <unordered_set>
#include "domain.hpp"
#include "value.hpp"

namespace stacksafe {

class MultiMap : private std::unordered_multimap<Value, Symbol> {
  using Super = std::unordered_multimap<Value, Symbol>;

 public:
  void insert(const Value &key, const Symbol &val);
  void insert(const Value &key, const Domain &val);
  void insert(const Symbol &key, const Symbol &val);
  void insert(const Symbol &key, const Domain &val);
  Domain lookup(const Value &key) const;
  Domain lookup(const Symbol &key) const;
  bool includes(const MultiMap &map) const;
  void merge(const MultiMap &map);
  static bool equals(const MultiMap &lhs, const MultiMap &rhs);
  static std::unordered_set<Value> keys(const MultiMap &map);
  static std::size_t hash(const MultiMap &map);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_A0BA2711_AA71_4105_83AF_E6AF119E4855
