#ifndef INCLUDE_GUARD_721DAB69_1C96_4A4D_BE1C_1C8B66A7065E
#define INCLUDE_GUARD_721DAB69_1C96_4A4D_BE1C_1C8B66A7065E

#include <memory>
#include <vector>

namespace stacksafe {
class Map;

class MapRef {
  const Map* map_;
  explicit MapRef(const Map& m);

 public:
  const Map& get() const;
};

using MapPtr = std::unique_ptr<Map>;

class MapPool : private std::vector<MapPtr> {
  using Super = std::vector<MapPtr>;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_721DAB69_1C96_4A4D_BE1C_1C8B66A7065E
