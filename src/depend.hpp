#ifndef INCLUDE_GUARD_DA5A1318_1109_4382_9A1C_7278AB44F4FA
#define INCLUDE_GUARD_DA5A1318_1109_4382_9A1C_7278AB44F4FA

#include <cstdint>
#include <vector>

namespace stacksafe {
class Symbol;

class Depend : private std::vector<std::uint8_t> {
  using Super = std::vector<std::uint8_t>;
  std::size_t size_;

 public:
  explicit Depend(std::size_t n);
  void set(const Symbol& key, const Symbol& val);

 private:
  void set(unsigned from, unsigned to);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_DA5A1318_1109_4382_9A1C_7278AB44F4FA
