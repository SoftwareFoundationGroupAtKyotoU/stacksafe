#include "stopwatch.hpp"

namespace stacksafe {

template <class Period>
Stopwatch<Period>::Stopwatch(double &elapsed)
    : elapsed_{elapsed}, start_{Clock::now()} {}
template <class Period>
Stopwatch<Period>::~Stopwatch() {
  auto end = Clock::now();
  Duration elapsed = end - start_;
  elapsed_ = elapsed.count();
}

template class Stopwatch<std::milli>;

}  // namespace stacksafe
