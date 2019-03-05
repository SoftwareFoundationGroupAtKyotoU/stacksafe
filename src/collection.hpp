#ifndef INCLUDE_GUARD_6495011D_0D6C_4DD2_8111_D661A86391FD
#define INCLUDE_GUARD_6495011D_0D6C_4DD2_8111_D661A86391FD

#include "visualize.hpp"
#include <algorithm>
#include <unordered_set>

namespace llvm {
  class raw_ostream;
}
namespace stacksafe {
  template <typename T>
  class Set : public std::unordered_set<T> {
  public:
    bool subsetof(const Set &rhs) const {
      return std::includes(begin(rhs), end(rhs), begin(*this), end(*this));
    }
    void unify(const Set &rhs) {
      this->insert(begin(rhs), end(rhs));
    }
    void print(llvm::raw_ostream &O) const {
      O << set_like(make_manip_seq(*this));
    }
  };
}

#endif//INCLUDE_GUARD_6495011D_0D6C_4DD2_8111_D661A86391FD
