#ifndef INCLUDE_GUARD_721DAB69_1C96_4A4D_BE1C_1C8B66A7065E
#define INCLUDE_GUARD_721DAB69_1C96_4A4D_BE1C_1C8B66A7065E

#include <functional>
#include <memory>
#include <vector>

namespace stacksafe {
class Env;
class FlatEnv;
class Map;

class MapRef {
  friend class MapPool;
  const Map* map_;
  explicit MapRef(const Map& m);

 public:
  const Map& get() const;
};
bool operator==(const MapRef& lhs, const MapRef& rhs);

using MapPtr = std::unique_ptr<Map>;

class MapPool : private std::vector<MapPtr> {
  using Super = std::vector<MapPtr>;

 public:
  MapRef add(const Map& m);
  Env add(const FlatEnv& e);
};

}  // namespace stacksafe

namespace std {
template <>
struct hash<stacksafe::MapRef> {
  size_t operator()(const stacksafe::MapRef& r) const;
};
}  // namespace std

#endif  // INCLUDE_GUARD_721DAB69_1C96_4A4D_BE1C_1C8B66A7065E
