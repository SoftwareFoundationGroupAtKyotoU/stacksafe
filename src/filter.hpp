#ifndef INCLUDE_GUARD_98FEBCB3_4573_41C9_AA40_98728267AB74
#define INCLUDE_GUARD_98FEBCB3_4573_41C9_AA40_98728267AB74

#include <bitset>
#include <climits>
#include <vector>

#define _USE_MATH_DEFINES
#include <cmath>
#undef _USE_MATH_DEFINES

namespace stacksafe {
class Map;

constexpr std::size_t block_width(std::size_t k) {
  const auto factor = static_cast<double>(k) / M_LN2 / CHAR_BIT;
  return static_cast<std::size_t>(factor) * CHAR_BIT;
}

class BloomFilter {
  static constexpr auto num_of_hash = 10;
  static constexpr auto width = block_width(num_of_hash);
  using Block = std::bitset<width>;
  using Indices = std::tuple<std::size_t, std::size_t>;
  std::vector<Block> buf_;

 public:
  explicit BloomFilter(std::size_t count);
  void add(const Map& map);
  void add(std::size_t hash);
  bool check(std::size_t hash) const;
  bool includes(const BloomFilter& filter) const;
  void merge(const BloomFilter& filter);

 private:
  void set(std::size_t index);
  bool test(std::size_t index) const;
  Indices calc_indices(std::size_t index) const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_98FEBCB3_4573_41C9_AA40_98728267AB74
