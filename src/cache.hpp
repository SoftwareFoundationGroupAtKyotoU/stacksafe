#ifndef INCLUDE_GUARD_FE9E67B8_3B2E_4541_A5BA_289DA3AF1C70
#define INCLUDE_GUARD_FE9E67B8_3B2E_4541_A5BA_289DA3AF1C70

#include <map>
#include "register.hpp"

namespace llvm {
class Value;
}

namespace stacksafe {

class RegisterCache : private std::map<const llvm::Value*, Register> {
  using Super = std::map<const llvm::Value*, Register>;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_FE9E67B8_3B2E_4541_A5BA_289DA3AF1C70
