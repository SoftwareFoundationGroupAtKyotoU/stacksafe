#ifndef INCLUDE_GUARD_C088E002_5D7C_41FC_929E_102BD43FCAEA
#define INCLUDE_GUARD_C088E002_5D7C_41FC_929E_102BD43FCAEA

#include <set>
#include "memory.hpp"

namespace llvm {
class Value;
}

namespace stacksafe {
class Domain;
class RegisterCache;
class Symbol;

using Params = std::set<const llvm::Value*>;

class Env {
  Memory mem_;
  RegisterCache& cache_;

 public:
  Env(const Memory& m, RegisterCache& c);
  Env(const llvm::Function& f, RegisterCache& c);
  Memory memory() const;
  Memory& memory();
  Domain lookup(const llvm::Value& key) const;
  Domain lookup(const Symbol& key) const;
  void binop(const llvm::Value& dst, const llvm::Value& lhs,
             const llvm::Value& rhs);
  void alloc(const llvm::Value& dst);
  void load(const llvm::Value& dst, const llvm::Value& src);
  void store(const llvm::Value& src, const llvm::Value& dst);
  void cmpxchg(const llvm::Value& dst, const llvm::Value& ptr,
               const llvm::Value& val);
  void cast(const llvm::Value& dst, const llvm::Value& src);
  void phi(const llvm::Value& dst, const Params& params);
  void call(const llvm::Value& dst, const Params& params);
  void constant(const llvm::Value& dst);

 private:
  void insert(const llvm::Value& key, const Domain& val);
  void insert(const Symbol& key, const Domain& val);
  void collect(const Symbol& symbol, Domain& done) const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_C088E002_5D7C_41FC_929E_102BD43FCAEA
