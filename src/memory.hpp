#ifndef INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA
#define INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA

#include <map>
#include "json_fwd.hpp"
#include "map.hpp"
#include "register.hpp"

namespace llvm {
class Function;
class Value;
}  // namespace llvm

namespace stacksafe {
class Fabric;

class Cache : private std::map<const llvm::Value *, Register> {
  using Super = std::map<const llvm::Value *, Register>;

 public:
  explicit Cache(const llvm::Function &f);
  Register lookup(const llvm::Value &key) const;

 private:
  void add(const llvm::Value &reg);
};

class Memory {
  using Heap = Map<Symbol>;
  using Stack = Map<Register>;
  Heap heap_;
  Stack stack_;

 public:
  const Heap &heap() const;
  const Stack &stack() const;
  bool includes(const Memory &that) const;
  bool merge(const Memory &that);
  Fabric diff(const Memory &that) const;
  Domain lookup(const Symbol &key) const;
  Domain lookup(const Register &key) const;
  void insert(const Symbol &key, const Domain &val);
  void insert(const Register &key, const Domain &val);
};
void to_json(Json &j, const Memory &x);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA
