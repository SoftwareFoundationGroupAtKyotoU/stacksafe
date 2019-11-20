#ifndef INCLUDE_GUARD_721DAB69_1C96_4A4D_BE1C_1C8B66A7065E
#define INCLUDE_GUARD_721DAB69_1C96_4A4D_BE1C_1C8B66A7065E

#include <memory>
#include <vector>

namespace stacksafe {
class Env;
class FlatEnv;
class FlatMap;
class MapRef;
class MultiMap;

class MapPtr : private std::unique_ptr<MultiMap> {
  friend class MapPool;
  using Super = std::unique_ptr<MultiMap>;
  explicit MapPtr(const FlatMap& flat);

 public:
  MapPtr(MapPtr&&);
  ~MapPtr();
  MapPtr& operator=(MapPtr&&);
  const MultiMap& get() const;
};
bool operator<(const MapPtr& lhs, const MapPtr& rhs);

class MapPool : private std::vector<MapPtr> {
  using Super = std::vector<MapPtr>;

 public:
  MapRef add(const FlatMap& flat);
  Env add(const FlatEnv& env);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_721DAB69_1C96_4A4D_BE1C_1C8B66A7065E
