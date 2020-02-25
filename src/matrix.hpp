#ifndef INCLUDE_GUARD_C0D10252_121F_463F_A84C_5DBD93E2DC5F
#define INCLUDE_GUARD_C0D10252_121F_463F_A84C_5DBD93E2DC5F

#include <cstdint>
#include <vector>

namespace stacksafe {

class Matrix : private std::vector<std::uint8_t> {
  using Super = std::vector<std::uint8_t>;
  std::size_t size_;

 public:
  explicit Matrix(std::size_t n);
  Matrix(std::size_t n, bool init);
  std::size_t size() const;
  void set(std::size_t row, std::size_t col);
  bool get(std::size_t row, std::size_t col) const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_C0D10252_121F_463F_A84C_5DBD93E2DC5F
