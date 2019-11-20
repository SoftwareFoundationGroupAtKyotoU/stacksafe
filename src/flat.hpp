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
class MapRef;
class Map;
class MapsTo;
class Map;
}  // namespace stacksafe

namespace std {
template <>
struct hash<stacksafe::MapRef> {
  size_t operator()(const stacksafe::MapRef& f) const;
};
}  // namespace std

namespace stacksafe {

class MapRef {
  const Map* flat_;

 public:
  explicit MapRef(const Map& flat);
  const Map& get() const;
};
bool operator==(const MapRef& lhs, const MapRef& rhs);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_9AD1204C_8632_4047_8AC2_467F02D9DFF8
