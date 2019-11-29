#ifndef INCLUDE_GUARD_DA5A1318_1109_4382_9A1C_7278AB44F4FA
#define INCLUDE_GUARD_DA5A1318_1109_4382_9A1C_7278AB44F4FA

#include <cstdint>
#include <vector>

namespace stacksafe {

class Depend : private std::vector<std::uint8_t> {
  using Super = std::vector<std::uint8_t>;

 public:
  explicit Depend(unsigned n);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_DA5A1318_1109_4382_9A1C_7278AB44F4FA
