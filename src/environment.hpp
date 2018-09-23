#ifndef INCLUDE_GUARD_52F60E3D_17C8_4A02_BEB6_64AF9019A2B4
#define INCLUDE_GUARD_52F60E3D_17C8_4A02_BEB6_64AF9019A2B4

#include <memory>
#include <vector>

namespace stacksafe {
  class LocationFactory {
    using Location = void *;
    std::vector<std::unique_ptr<char>> local_;
    static char outlive_, global_, undefined_;
  public:
    Location getLocal();
    Location getOutlive();
    Location getGlobal();
    Location getUndefined();
  };
}

#endif//INCLUDE_GUARD_52F60E3D_17C8_4A02_BEB6_64AF9019A2B4
