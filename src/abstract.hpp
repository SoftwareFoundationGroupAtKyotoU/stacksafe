#ifndef INCLUDE_GUARD_0E783D0B_1A5B_4270_A79B_0594D4E3D6CB
#define INCLUDE_GUARD_0E783D0B_1A5B_4270_A79B_0594D4E3D6CB

#include <map>
#include <memory>
#include <optional>
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
  std::map<const llvm::BasicBlock*, Env> blocks_;
  Log& log_;
  friend void to_json(Json& j, const Abstract& x);

 public:
  explicit Abstract(const llvm::Function& f, Log& log);
  static std::unique_ptr<Log> interpret(const llvm::Function& f);

 private:
  std::optional<Env> update(const llvm::BasicBlock* b, const Env& pred);
  void interpret(const llvm::BasicBlock* b, const Env& pred);
};
void to_json(Json& j, const Abstract& x);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_0E783D0B_1A5B_4270_A79B_0594D4E3D6CB
