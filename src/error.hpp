#ifndef INCLUDE_GUARD_FDDC1B6C_FE27_4C3E_B56D_5A2207D86792
#define INCLUDE_GUARD_FDDC1B6C_FE27_4C3E_B56D_5A2207D86792

#include <cstddef>

namespace stacksafe {

class Error {
  using Base = std::size_t;
  enum class Kind : Base { RETURN, CALL, GLOBAL, ARGUMENT };
  Base flags_;

 public:
  Error();
  bool is_error() const;

 private:
  static Base shift(Kind k);
  void set(Kind k);
  bool get(Kind k) const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_FDDC1B6C_FE27_4C3E_B56D_5A2207D86792
