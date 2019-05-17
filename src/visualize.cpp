#include "visualize.hpp"
#include <llvm/Support/Debug.h>

namespace stacksafe {
  Manip::Manip(std::size_t s) {
    this->reserve(s);
  }

  void make_manip_aux(Manip &)
  {}
  Manip join(const Manip &sep, const Manip &m) {
    using std::begin;
    using std::end;
    Manip ret;
    if (!m.empty()) {
      ret.reserve(m.size() * 2 - 1);
      auto it = begin(m);
      ret.push_back(*it);
      for (++it; it != end(m); ++it) {
        ret.push_back(wrap(sep));
        ret.push_back(*it);
      }
    }
    return ret;
  }
  llvm::raw_ostream &operator<<(llvm::raw_ostream &O, const Manip &x) {
    for (const auto &e : x) {
      e(O);
    }
    return O;
  }

  llvm::raw_ostream &debugs() {
    if (::llvm::DebugFlag && ::llvm::isCurrentDebugType("")) {
      return llvm::dbgs();
    } else {
      return llvm::nulls();
    }
  }
}
