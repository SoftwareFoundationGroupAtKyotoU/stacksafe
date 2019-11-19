#ifndef INCLUDE_GUARD_0E783D0B_1A5B_4270_A79B_0594D4E3D6CB
#define INCLUDE_GUARD_0E783D0B_1A5B_4270_A79B_0594D4E3D6CB

#include <string>
#include "blocks.hpp"
#include "error.hpp"
#include "log.hpp"

namespace llvm {
class Function;
class raw_ostream;
}  // namespace llvm

namespace stacksafe {

class Abstract {
  const Log log_;
  Error error_;
  Blocks blocks_;
  std::string name_;
  double elapsed_;

 public:
  explicit Abstract(const llvm::Function &f);
  void run(const llvm::Function &f);
  void print(llvm::raw_ostream &os) const;

 private:
  void interpret(const llvm::BasicBlock &b, DoubleMap prev);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_0E783D0B_1A5B_4270_A79B_0594D4E3D6CB
