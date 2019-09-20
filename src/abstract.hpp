#ifndef INCLUDE_GUARD_0E783D0B_1A5B_4270_A79B_0594D4E3D6CB
#define INCLUDE_GUARD_0E783D0B_1A5B_4270_A79B_0594D4E3D6CB

#include <map>
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

class Blocks : private std::map<const llvm::BasicBlock*, Env> {
  using Super = std::map<const llvm::BasicBlock*, Env>;

 public:
  using Super::begin, Super::end;
  void init(const llvm::BasicBlock* b);
  const Env& get(const llvm::BasicBlock* b) const;
  Env& get(const llvm::BasicBlock* b);
};

class Abstract {
  Blocks blocks_;
  const llvm::Function& func_;
  bool safe_;
  std::optional<Log> log_;

 public:
  explicit Abstract(const llvm::Function& f);
  const Blocks& blocks() const;
  void interpret();
  void verify();
  void print(llvm::raw_ostream& os) const;

 private:
  void interpret(const llvm::BasicBlock* b, const Env& pred);
  std::optional<Env> update(const llvm::BasicBlock* b, const Env& pred);
};
void to_json(Json& j, const Abstract& x);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_0E783D0B_1A5B_4270_A79B_0594D4E3D6CB
