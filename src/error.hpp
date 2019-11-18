#ifndef INCLUDE_GUARD_FDDC1B6C_FE27_4C3E_B56D_5A2207D86792
#define INCLUDE_GUARD_FDDC1B6C_FE27_4C3E_B56D_5A2207D86792

#include <cstddef>

namespace stacksafe {

class Error {
  using Base = std::size_t;
  enum class Kind : Base {
    RETURN = 1 << 0,
    CALL = 1 << 1,
    GLOBAL = 1 << 2,
    ARGUMENT = 1 << 3
  };
  Base flags_;

 public:
  Error();
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_FDDC1B6C_FE27_4C3E_B56D_5A2207D86792
