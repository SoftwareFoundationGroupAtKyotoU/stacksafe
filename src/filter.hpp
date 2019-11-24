#ifndef INCLUDE_GUARD_98FEBCB3_4573_41C9_AA40_98728267AB74
#define INCLUDE_GUARD_98FEBCB3_4573_41C9_AA40_98728267AB74

#include <bitset>
#include <climits>
#include <vector>

#define _USE_MATH_DEFINES
#include <cmath>
#undef _USE_MATH_DEFINES

namespace stacksafe {

constexpr std::size_t block_width(std::size_t k) {
  const auto factor = static_cast<double>(k) / M_LN2 / CHAR_BIT;
  return static_cast<std::size_t>(factor) * CHAR_BIT;
}

class BloomFilter {
  static constexpr auto num_of_hash = 10;
  static constexpr auto width = block_width(num_of_hash);
  using Block = std::bitset<width>;
  std::vector<Block> buf_;

 public:
  explicit BloomFilter(std::size_t count);
  void add(std::size_t hash);

 private:
  void set(std::size_t once, std::size_t twice, std::size_t nth);
  bool test(std::size_t once, std::size_t twice, std::size_t nth) const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_98FEBCB3_4573_41C9_AA40_98728267AB74
