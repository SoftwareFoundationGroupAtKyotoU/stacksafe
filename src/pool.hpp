#ifndef INCLUDE_GUARD_721DAB69_1C96_4A4D_BE1C_1C8B66A7065E
#define INCLUDE_GUARD_721DAB69_1C96_4A4D_BE1C_1C8B66A7065E

#include <memory>
#include <vector>

namespace stacksafe {
class Env;
class Map;
class MapRef;

class MapPtr : private std::unique_ptr<Map> {
  friend class MapPool;
  using Super = std::unique_ptr<Map>;
  explicit MapPtr(const Map& map);

 public:
  MapPtr(MapPtr&&);
  ~MapPtr();
  MapPtr& operator=(MapPtr&&);
  const Map& get() const;
};
bool operator<(const MapPtr& lhs, const MapPtr& rhs);

class MapPool : private std::vector<MapPtr> {
  using Super = std::vector<MapPtr>;

 public:
  MapRef add(const Map& map);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_721DAB69_1C96_4A4D_BE1C_1C8B66A7065E
