#ifndef INCLUDE_GUARD_9AD1204C_8632_4047_8AC2_467F02D9DFF8
#define INCLUDE_GUARD_9AD1204C_8632_4047_8AC2_467F02D9DFF8

#include <functional>
#include <memory>
#include <unordered_set>
#include <vector>
#include "symbol.hpp"
#include "value.hpp"

namespace stacksafe {
class Domain;
class FlatMapRef;
class Map;
class MapsTo;
class MultiMap;
}  // namespace stacksafe

namespace std {
template <>
struct hash<stacksafe::MapsTo> {
  size_t operator()(const stacksafe::MapsTo& to) const;
};
template <>
struct hash<stacksafe::FlatMapRef> {
  size_t operator()(const stacksafe::FlatMapRef& f) const;
};
}  // namespace std

namespace stacksafe {

class MapsTo : private std::tuple<Value, Symbol> {
  using Super = std::tuple<Value, Symbol>;

 public:
  MapsTo(const Value& val, const Symbol& sym);
  const Value& key() const;
  const Symbol& val() const;
  static std::size_t hash(const MapsTo& to);
};
bool operator==(const MapsTo& lhs, const MapsTo& rhs);

class FlatMap : private std::unordered_set<MapsTo> {
  using Super = std::unordered_set<MapsTo>;

 public:
  void insert(const Value& key, const Symbol& val);
  void insert(const Value& key, const Domain& val);
  void merge(const FlatMap& flat);
  bool includes(const FlatMap& flat) const;
  static Map to_map(const FlatMap& flat);
  static size_t hash(const FlatMap& flat);
  static bool equals(const FlatMap& lhs, const FlatMap& rhs);
  MultiMap to_multi() const;
};

class FlatMapRef {
  const MultiMap* flat_;

 public:
  explicit FlatMapRef(const MultiMap& flat);
  const MultiMap& get() const;
};
bool operator==(const FlatMapRef& lhs, const FlatMapRef& rhs);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_9AD1204C_8632_4047_8AC2_467F02D9DFF8
