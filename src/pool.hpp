#ifndef INCLUDE_GUARD_721DAB69_1C96_4A4D_BE1C_1C8B66A7065E
#define INCLUDE_GUARD_721DAB69_1C96_4A4D_BE1C_1C8B66A7065E

#include <memory>
#include <vector>
#include "map.hpp"

namespace stacksafe {

class MapPtr : private std::unique_ptr<Map> {
  friend class MapPool;
  using Super = std::unique_ptr<Map>;
  explicit MapPtr(const Map& map);

 public:
  MapPtr(MapPtr&&);
  ~MapPtr();
  MapPtr& operator=(MapPtr&&);
  const Map& get() const;
  Map& get();
};

class MapPool : private std::vector<MapPtr> {
  using Super = std::vector<MapPtr>;

 public:
  MapRef add(const Map& map);
  void remove(const MapRef& ref);
};

class EnvOld : private std::vector<MapRef> {
  using Super = std::vector<MapRef>;

 public:
  EnvOld() = default;
  EnvOld(MapRef ref);
  Map concat() const;
  void merge(const EnvOld& env);
  void insert(MapRef ref);
  bool element(MapRef ref) const;
  bool element(const Value& key, const Value& val) const;
  bool includes(const EnvOld& env) const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_721DAB69_1C96_4A4D_BE1C_1C8B66A7065E
