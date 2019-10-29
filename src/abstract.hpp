#ifndef INCLUDE_GUARD_0E783D0B_1A5B_4270_A79B_0594D4E3D6CB
#define INCLUDE_GUARD_0E783D0B_1A5B_4270_A79B_0594D4E3D6CB

#include <memory>
#include "blocks.hpp"
#include "json_fwd.hpp"

namespace llvm {
class Function;
class raw_ostream;
}  // namespace llvm

namespace stacksafe {

class Abstract {
  Blocks blocks_;
  const llvm::Function &func_;
  bool safe_;

 public:
  explicit Abstract(const llvm::Function &f);
  const Blocks &blocks() const;
  void interpret();
  void verify();
  void print(llvm::raw_ostream &os) const;

 private:
  void interpret(const llvm::BasicBlock &b);
};
void to_json(Json &j, const Abstract &x);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_0E783D0B_1A5B_4270_A79B_0594D4E3D6CB
