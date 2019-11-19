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
class Map;
class MapsTo;
}  // namespace stacksafe

namespace std {
template <>
struct hash<stacksafe::MapsTo> {
  size_t operator()(const stacksafe::MapsTo& to) const;
};
}  // namespace std

namespace stacksafe {

class MapsTo : private std::tuple<Value, Symbol> {
  using Super = std::tuple<Value, Symbol>;

 public:
  MapsTo(const Value& val, const Symbol& sym);
  const Value& value() const;
  const Symbol& symbol() const;
  static std::size_t hash(const MapsTo& to);
};
bool operator==(const MapsTo& lhs, const MapsTo& rhs);

class FlatMap : private std::unordered_set<MapsTo> {
  using Super = std::unordered_set<MapsTo>;

 public:
  void insert(const Value& key, const Symbol& val);
  void insert(const Value& key, const Domain& val);
  void insert(const Map& map);
  bool includes(const FlatMap& flat) const;
  static size_t hash(const FlatMap& flat);
  static bool equals(const FlatMap& lhs, const FlatMap& rhs);
};

class FlatMapPtr : private std::unique_ptr<FlatMap> {
  friend class FlatMapPool;
  using Super = std::unique_ptr<FlatMap>;
  explicit FlatMapPtr(const FlatMap& flat);

 public:
  const FlatMap& get() const;
};
bool operator<(const FlatMapPtr& lhs, const FlatMapPtr& rhs);

class FlatMapRef {
  friend class FlatMapPool;
  const FlatMap* flat_;
  explicit FlatMapRef(const FlatMap& flat);

 public:
  const FlatMap& get() const;
};
bool operator==(const FlatMapRef& lhs, const FlatMapRef& rhs);

class FlatMapPool : private std::vector<FlatMapPtr> {
  using Super = std::vector<FlatMapPtr>;

 public:
  FlatMapRef add(const FlatMap& flat);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_9AD1204C_8632_4047_8AC2_467F02D9DFF8
