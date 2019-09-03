#ifndef INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA
#define INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA

#include <vector>
#include "json_fwd.hpp"
#include "map.hpp"

namespace llvm {
class Function;
class Use;
}  // namespace llvm

namespace stacksafe {
class Value;

class Env {
  Heap heap_;
  Stack stack_;
  friend void to_json(Json& j, const Env& x);

 public:
  Env() = default;
  explicit Env(llvm::Function& f);
  bool merge(const Env& that);
  void argument(const Value& arg);
  void alloc(const Value& dst);
  void store(const Value& src, const Value& dst);
  void load(const Value& dst, const Value& src);
  void constant(const Value& dst);
  void call(const Value& dst, const std::vector<llvm::Use*>& params);
};
void to_json(Json& j, const Env& x);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA
