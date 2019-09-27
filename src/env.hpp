#ifndef INCLUDE_GUARD_C088E002_5D7C_41FC_929E_102BD43FCAEA
#define INCLUDE_GUARD_C088E002_5D7C_41FC_929E_102BD43FCAEA

namespace stacksafe {
class Memory;
class RegisterCache;

class Env {
  Memory& mem_;
  RegisterCache& cache_;

 public:
  Env(Memory& m, RegisterCache& r);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_C088E002_5D7C_41FC_929E_102BD43FCAEA
