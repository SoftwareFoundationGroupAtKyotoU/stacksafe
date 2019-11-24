#ifndef INCLUDE_GUARD_98FEBCB3_4573_41C9_AA40_98728267AB74
#define INCLUDE_GUARD_98FEBCB3_4573_41C9_AA40_98728267AB74

#include <climits>

#define _USE_MATH_DEFINES
#include <cmath>
#undef _USE_MATH_DEFINES

namespace stacksafe {

constexpr std::size_t block_length(std::size_t num_of_hash) {
  const auto factor = static_cast<double>(num_of_hash) / M_LN2 / CHAR_BIT;
  return static_cast<std::size_t>(factor) * CHAR_BIT;
}

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_98FEBCB3_4573_41C9_AA40_98728267AB74
