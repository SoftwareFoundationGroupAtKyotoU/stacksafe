#ifndef INCLUDE_GUARD_CDCF6693_F2F5_43D1_B469_AAC25E0E135E
#define INCLUDE_GUARD_CDCF6693_F2F5_43D1_B469_AAC25E0E135E

#include <chrono>

namespace stacksafe {

template <class Period>
class Stopwatch {
  using Clock = std::chrono::high_resolution_clock;
  using Duration = std::chrono::duration<double, Period>;
  double &elapsed_;
  decltype(Clock::now()) start_;

 public:
  explicit Stopwatch(double &elapsed)
      : elapsed_{elapsed}, start_{Clock::now()} {}
  ~Stopwatch() {
    auto end = Clock::now();
    Duration elapsed = end - start_;
    elapsed_ = elapsed.count();
  }
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_CDCF6693_F2F5_43D1_B469_AAC25E0E135E
