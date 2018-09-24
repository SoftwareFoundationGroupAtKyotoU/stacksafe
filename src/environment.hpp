#ifndef INCLUDE_GUARD_52F60E3D_17C8_4A02_BEB6_64AF9019A2B4
#define INCLUDE_GUARD_52F60E3D_17C8_4A02_BEB6_64AF9019A2B4

#include <functional>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace llvm {
  class Value;
}
namespace stacksafe {
  class Location : private std::hash<const void *> {
    using Base = std::hash<const void *>;
    friend class LocationFactory;
    const void *const loc_;
    explicit Location(const char &c);
  public:
    size_t hash() const;
  };
  class LocationFactory {
    std::vector<std::unique_ptr<char>> local_;
    static char outlive_, global_, undefined_;
  public:
    Location getLocal();
    Location getOutlive();
    Location getGlobal();
    Location getUndefined();
  };
  using AbstractType = std::unordered_set<Location>;
  class HeapMap {
    std::unordered_map<Location, AbstractType> map_;
  };
  class EnvMap {
    std::unordered_map<llvm::Value *, AbstractType> map_;
  };
}

#endif//INCLUDE_GUARD_52F60E3D_17C8_4A02_BEB6_64AF9019A2B4
