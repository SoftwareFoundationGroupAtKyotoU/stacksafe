#ifndef INCLUDE_GUARD_721DAB69_1C96_4A4D_BE1C_1C8B66A7065E
#define INCLUDE_GUARD_721DAB69_1C96_4A4D_BE1C_1C8B66A7065E

#include <memory>
#include <vector>

namespace stacksafe {
class Env;
class FlatEnv;
class FlatMap;
class FlatMapRef;
class MultiMap;

class MapPtr : private std::unique_ptr<MultiMap> {
  friend class FlatMapPool;
  using Super = std::unique_ptr<MultiMap>;
  explicit MapPtr(const FlatMap& flat);

 public:
  MapPtr(MapPtr&&);
  ~MapPtr();
  MapPtr& operator=(MapPtr&&);
  const MultiMap& get() const;
};
bool operator<(const MapPtr& lhs, const MapPtr& rhs);

class FlatMapPool : private std::vector<MapPtr> {
  using Super = std::vector<MapPtr>;

 public:
  FlatMapRef add(const FlatMap& flat);
  Env add(const FlatEnv& env);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_721DAB69_1C96_4A4D_BE1C_1C8B66A7065E
