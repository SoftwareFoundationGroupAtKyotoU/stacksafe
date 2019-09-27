#ifndef INCLUDE_GUARD_C088E002_5D7C_41FC_929E_102BD43FCAEA
#define INCLUDE_GUARD_C088E002_5D7C_41FC_929E_102BD43FCAEA

#include <set>
#include "memory.hpp"

namespace llvm {
class Value;
}

namespace stacksafe {
class Domain;
class Params;
class RegisterCache;
class Symbol;

using ValueSet = std::set<const llvm::Value*>;

class Env {
  Memory mem_;

 public:
  explicit Env(const Memory& m);
  Memory memory() const;
  Memory& memory();
  void insert_stack(const llvm::Value& key, const Domain& val);
  void insert_heap(const Symbol& key, const Domain& val);
  Domain from_stack(const llvm::Value& key) const;
  Domain from_heap(const Symbol& key) const;
  Domain collect(const ValueSet& params) const;

 private:
  void collect(const Symbol& symbol, Domain& done) const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_C088E002_5D7C_41FC_929E_102BD43FCAEA
