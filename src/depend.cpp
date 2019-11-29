#include "depend.hpp"

namespace stacksafe {

Depend::Depend(unsigned n) : Super((n + 2) * n), size_{n} {
  for (unsigned i = 0; i < n; ++i) {
    set(i, i);
  }
}
void Depend::set(unsigned from, unsigned to) {
  Super::at((size_ + 2) * from + to) = 1;
}

}  // namespace stacksafe
