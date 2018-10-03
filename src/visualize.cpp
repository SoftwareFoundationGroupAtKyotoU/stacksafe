#include "visualize.hpp"

namespace stacksafe {
  Manip::Manip(std::size_t s) {
    this->reserve(s);
  }
  Manip::Manip(std::initializer_list<ManipObj> l)
    : Manip(l.size()) {
    for (const auto &e : l) {
      this->push_back(e);
    }
  }
  void Manip::print(llvm::raw_ostream &O) const {
    for (const auto &e : *this) {
      e(O);
    }
  }

  void make_manip_aux(Manip &)
  {}
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
  Manip join(const ManipObj &sep, const Manip &m) {
    using std::begin;
    using std::end;
    Manip ret;
    if (!m.empty()) {
      ret.reserve(m.size() * 2 - 1);
      auto it = begin(m);
      ret.push_back(*it);
      for (++it; it != end(m); ++it) {
        ret.push_back(sep);
        ret.push_back(*it);
      }
    }
    return ret;
  }
  Manip set_like(const Manip &m) {
    return braces(join(wrap(", "), m));
  }
}
