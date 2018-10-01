#include "visualize.hpp"

namespace stacksafe {
  Manip::Manip(std::initializer_list<ManipObj> l) {
    this->reserve(l.size());
    for (const auto &e : l) {
      this->push_back(e);
    }
  }
}
