#ifndef INCLUDE_GUARD_721DAB69_1C96_4A4D_BE1C_1C8B66A7065E
#define INCLUDE_GUARD_721DAB69_1C96_4A4D_BE1C_1C8B66A7065E

#include <memory>
#include <vector>

namespace stacksafe {
class Env;
class FlatEnv;
class Map;
class MapRef;

class MapPtr : private std::unique_ptr<Map> {
  using Super = std::unique_ptr<Map>;
};

class MapPool : private std::vector<std::unique_ptr<Map>> {
  using Super = std::vector<std::unique_ptr<Map>>;
  MapRef add(const Map& m);

 public:
  Env add(const FlatEnv& e);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_721DAB69_1C96_4A4D_BE1C_1C8B66A7065E
