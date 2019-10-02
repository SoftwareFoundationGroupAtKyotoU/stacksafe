#ifndef INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA
#define INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA

#include <map>
#include "json_fwd.hpp"
#include "map.hpp"
#include "register.hpp"

namespace llvm {
class Value;
}

namespace stacksafe {

class Cache : private std::map<const llvm::Value*, Register> {
  using Super = std::map<const llvm::Value*, Register>;

 public:
  Register lookup(const llvm::Value& key);
  void add(const llvm::Value& reg);
};

class Memory {
  using Heap = Map<Symbol>;
  using Stack = Map<Register>;
  Heap heap_;
  Stack stack_;

 public:
  Memory() = default;
  const Heap& heap() const;
  Heap& heap();
  const Stack& stack() const;
  Stack& stack();
  bool includes(const Memory& that) const;
  bool merge(const Memory& that);
};
void to_json(Json& j, const Memory& x);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA
