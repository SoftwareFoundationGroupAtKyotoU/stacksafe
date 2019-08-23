#include "visualize.hpp"
#include <llvm/Support/Debug.h>

namespace stacksafe {
Manipulator::Manipulator() {}
void Manipulator::print(llvm::raw_ostream &O) const {
  for (auto &e : *this) {
    e(O);
  }
}
void Manipulator::manip_impl() {}

llvm::raw_ostream &debugs(const char *type) {
  DEBUG_WITH_TYPE(type, return llvm::dbgs());
  return llvm::nulls();
}
}  // namespace stacksafe
