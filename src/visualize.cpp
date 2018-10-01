#include "visualize.hpp"

namespace stacksafe {
  Manip::Manip(std::initializer_list<ManipObj> l) {
    this->reserve(l.size());
    for (const auto &e : l) {
      this->push_back(e);
    }
  }
  void Manip::print(llvm::raw_ostream &O) const {
    for (const auto &e : *this) {
      e(O);
    }
  }
}
