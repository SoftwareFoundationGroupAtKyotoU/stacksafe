#ifndef INCLUDE_GUARD_53770CF6_24DC_46BB_B928_E142E873335B
#define INCLUDE_GUARD_53770CF6_24DC_46BB_B928_E142E873335B

#include <string>

namespace llvm {
class Type;
}

namespace stacksafe {

std::string to_str(const llvm::Type& type);

}

#endif  // INCLUDE_GUARD_53770CF6_24DC_46BB_B928_E142E873335B
