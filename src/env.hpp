#ifndef INCLUDE_GUARD_C088E002_5D7C_41FC_929E_102BD43FCAEA
#define INCLUDE_GUARD_C088E002_5D7C_41FC_929E_102BD43FCAEA

#include <set>
#include "memory.hpp"

namespace llvm {
class Function;
class Value;
}  // namespace llvm

namespace stacksafe {
class Domain;
class Symbol;

class Params : private std::set<const llvm::Value*> {
  using Super = std::set<const llvm::Value*>;

 public:
  Params() = default;
  using Super::begin, Super::end;
  void insert(const llvm::Value& v);
};

class Env {
  Cache& cache_;
  Memory mem_;

 public:
  explicit Env(Cache& c);
  Env(Cache& c, const Memory& m);
  Env(Cache& c, const llvm::Function& f);
  Memory memory() const;
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
  Domain lookup(const llvm::Value& key) const;
  Domain lookup(const Symbol& key) const;
  void insert(const llvm::Value& key, const Domain& val);
  void insert(const Symbol& key, const Domain& val);
  void collect(const Symbol& symbol, Domain& done) const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_C088E002_5D7C_41FC_929E_102BD43FCAEA
