#ifndef INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA
#define INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA

#include "json_fwd.hpp"
#include "map.hpp"
#include "register.hpp"

namespace llvm {
class Function;
class Value;
}  // namespace llvm

namespace stacksafe {
class Cache;

class Memory {
  const Cache &cache_;
  Map heap_, stack_;

 public:
  explicit Memory(const Cache &c);
  Memory(const Cache &c, const llvm::Function &f);
  const Map &heap() const;
  const Map &stack() const;
  bool includes(const Memory &that) const;
  void merge(const Memory &that);
  const Domain &lookup(const Register &key) const;
  const Domain &lookup(const llvm::Value &key) const;
  void insert(const Register &key, const Domain &val);
  void insert(const llvm::Value &key, const Domain &val);
  Register alloc(const llvm::Value &key);
  void collect(const Register &curr, Domain &done) const;
};
void to_json(Json &j, const Memory &x);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_FCDC6E4A_7148_4D58_920E_D9467F2A6CBA
