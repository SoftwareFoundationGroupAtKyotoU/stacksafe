#ifndef INCLUDE_GUARD_0E783D0B_1A5B_4270_A79B_0594D4E3D6CB
#define INCLUDE_GUARD_0E783D0B_1A5B_4270_A79B_0594D4E3D6CB

#include <map>
#include <vector>
#include "env.hpp"
#include "json_fwd.hpp"

namespace llvm {
class BasicBlock;
class Function;
}  // namespace llvm

namespace stacksafe {
class Log;

class Abstract {
  const llvm::Function* const func_;
  std::map<const llvm::BasicBlock*, Env> blocks_, prevs_;
  Log& log_;
  friend void to_json(Json& j, const Abstract& x);

 public:
  explicit Abstract(const llvm::Function& f, Log& log);
  void interpret();

 private:
  void interpret(const llvm::BasicBlock* b, const Env& e);
  bool update(const llvm::BasicBlock* b, const Env& e);
};
void to_json(Json& j, const Abstract& x);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_0E783D0B_1A5B_4270_A79B_0594D4E3D6CB
