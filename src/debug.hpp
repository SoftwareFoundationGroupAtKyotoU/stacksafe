#ifndef INCLUDE_GUARD_91A166AC_CDE2_42E8_B393_EEBD12227F6A
#define INCLUDE_GUARD_91A166AC_CDE2_42E8_B393_EEBD12227F6A

#include <vector>
#include "env.hpp"

namespace llvm {
class BasicBlock;
class Function;
class Instruction;
class raw_ostream;
}  // namespace llvm

namespace stacksafe {

struct LogBlock {
  const llvm::BasicBlock* const block;
  const Env prev, next;

 public:
  LogBlock(const llvm::BasicBlock* b, const Env& p, const Env& n);
  void print(llvm::raw_ostream& os) const;
};

struct Log {
  const llvm::Function* const function;
  std::string filename;
  std::vector<LogBlock> blocks;

 public:
  explicit Log(const llvm::Function& func);
  void add(const llvm::BasicBlock* block, const Env& prev, const Env& next);
  void print(llvm::raw_ostream& os) const;
};

void unknown_instruction(llvm::Instruction& i);
void check_interpret(llvm::Instruction& i, bool success);
void missing_log(llvm::raw_ostream& os);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_91A166AC_CDE2_42E8_B393_EEBD12227F6A
