#ifndef INCLUDE_GUARD_0E783D0B_1A5B_4270_A79B_0594D4E3D6CB
#define INCLUDE_GUARD_0E783D0B_1A5B_4270_A79B_0594D4E3D6CB

#include <map>
#include <memory>
#include <optional>
#include "debug.hpp"
#include "env.hpp"
#include "json_fwd.hpp"

namespace llvm {
class BasicBlock;
class Function;
class raw_ostream;
}  // namespace llvm

namespace stacksafe {
struct Log;

class Abstract {
  std::map<const llvm::BasicBlock*, Env> blocks_;
  const llvm::Function& func_;
  Log log_;
  friend void to_json(Json& j, const Abstract& x);

 private:
  std::optional<Env> update(const llvm::BasicBlock* b, const Env& pred);
  void interpret(const llvm::BasicBlock* b, const Env& pred);

 public:
  explicit Abstract(const llvm::Function& f);
  static std::unique_ptr<Log> interpret(const llvm::Function& f);
  void print(llvm::raw_ostream& os) const;
};
void to_json(Json& j, const Abstract& x);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_0E783D0B_1A5B_4270_A79B_0594D4E3D6CB
