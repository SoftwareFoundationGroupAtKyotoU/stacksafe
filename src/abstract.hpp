#ifndef INCLUDE_GUARD_0E783D0B_1A5B_4270_A79B_0594D4E3D6CB
#define INCLUDE_GUARD_0E783D0B_1A5B_4270_A79B_0594D4E3D6CB

#include "depend.hpp"

namespace llvm {
class Function;
class raw_ostream;
}  // namespace llvm

namespace stacksafe {

class Abstract {
  const llvm::Function &func_;
  Depend depend_;
  DependMap depmap_;
  double elapsed_;

 public:
  explicit Abstract(const llvm::Function &f);
  void interpret();
  void print(llvm::raw_ostream &os) const;
  void print_depend(llvm::raw_ostream &os) const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_0E783D0B_1A5B_4270_A79B_0594D4E3D6CB
