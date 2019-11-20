#ifndef INCLUDE_GUARD_A0BA2711_AA71_4105_83AF_E6AF119E4855
#define INCLUDE_GUARD_A0BA2711_AA71_4105_83AF_E6AF119E4855

#include <functional>
#include <unordered_map>
#include "domain.hpp"
#include "value.hpp"

namespace stacksafe {

class MultiMap : private std::unordered_multimap<Value, Symbol> {
  using Super = std::unordered_multimap<Value, Symbol>;

 public:
  void insert(const Value &key, const Symbol &val);
  void insert(const Value &key, const Domain &val);
  Domain lookup(const Value &key) const;
  void merge(const MultiMap &map);
};

class Map : private std::unordered_map<Value, Domain> {
  using Super = std::unordered_map<Value, Domain>;

 public:
  using Super::begin, Super::end;
  void insert(const Symbol &key, const Domain &val);
  void insert(const Value &key, const Domain &val);
  void insert(const Value &key, const Symbol &val);
  Domain lookup(const Symbol &key) const;
  Domain lookup(const Value &key) const;
  void merge(const Map &that);
};

class Heap : private Map {
 public:
  explicit Heap(const Map &m);
  void insert(const Symbol &key, const Domain &val);
  Domain lookup(const Symbol &key) const;
  void merge(const Heap &that);
};
class Stack : private Map {
 public:
  using Map::insert, Map::lookup, Map::merge;
  explicit Stack(const Map &m);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_A0BA2711_AA71_4105_83AF_E6AF119E4855
