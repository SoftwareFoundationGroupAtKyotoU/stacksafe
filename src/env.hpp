#ifndef INCLUDE_GUARD_C088E002_5D7C_41FC_929E_102BD43FCAEA
#define INCLUDE_GUARD_C088E002_5D7C_41FC_929E_102BD43FCAEA

namespace llvm {
class Value;
}

namespace stacksafe {
class Domain;
class Memory;
class RegisterCache;

class Env {
  Memory& mem_;
  RegisterCache& cache_;

 public:
  Env(Memory& m, RegisterCache& r);
  void insert_stack(const llvm::Value& key, const Domain& val);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_C088E002_5D7C_41FC_929E_102BD43FCAEA
