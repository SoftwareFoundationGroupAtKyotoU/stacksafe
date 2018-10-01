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

  Manip parens(const Manip &m) {
    return Manip{ wrap("("), wrap(m), wrap(")") };
  }
  Manip braces(const Manip &m) {
    return Manip{ wrap("{"), wrap(m), wrap("}") };
  }
  Manip squares(const Manip &m) {
    return Manip{ wrap("["), wrap(m), wrap("]") };
  }
  Manip angles(const Manip &m) {
    return Manip{ wrap("<"), wrap(m), wrap(">") };
  }
}
