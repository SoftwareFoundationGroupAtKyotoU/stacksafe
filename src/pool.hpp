#ifndef INCLUDE_GUARD_721DAB69_1C96_4A4D_BE1C_1C8B66A7065E
#define INCLUDE_GUARD_721DAB69_1C96_4A4D_BE1C_1C8B66A7065E

#include <functional>
#include <memory>
#include <vector>

namespace stacksafe {
class EnvOld;
class FlatEnvOld;
class Map;
class MapRef;

class MapPtr : private std::unique_ptr<Map> {
  friend class MapPool;
  using Super = std::unique_ptr<Map>;
  explicit MapPtr(const Map& m);

 public:
  const Map& get() const;
};
bool operator<(const MapPtr& lhs, const MapPtr& rhs);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_721DAB69_1C96_4A4D_BE1C_1C8B66A7065E
