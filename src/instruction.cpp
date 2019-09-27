#include "instruction.hpp"
#include <cassert>
#include "env.hpp"
#include "memory.hpp"

namespace stacksafe {
namespace instr {

void constant(Env& e, const llvm::Value& dst) { e.insert_stack(dst, Domain{}); }

}  // namespace instr
}  // namespace stacksafe
