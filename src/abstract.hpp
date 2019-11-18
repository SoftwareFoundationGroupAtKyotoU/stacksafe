#ifndef INCLUDE_GUARD_0E783D0B_1A5B_4270_A79B_0594D4E3D6CB
#define INCLUDE_GUARD_0E783D0B_1A5B_4270_A79B_0594D4E3D6CB

#include <string>
#include "blocks.hpp"
#include "error.hpp"
#include "safe.hpp"

namespace llvm {
class Function;
class raw_ostream;
}  // namespace llvm

namespace stacksafe {

class Abstract {
  Error error_;
  Blocks blocks_;
  Safe safe_;
  std::string name_;
  double elapsed_;

 public:
  explicit Abstract(const llvm::Function &f);
  void run(const llvm::Function &f);
  void print(llvm::raw_ostream &os) const;

 private:
  void interpret(const llvm::BasicBlock &b);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_0E783D0B_1A5B_4270_A79B_0594D4E3D6CB
