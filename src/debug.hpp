#ifndef INCLUDE_GUARD_91A166AC_CDE2_42E8_B393_EEBD12227F6A
#define INCLUDE_GUARD_91A166AC_CDE2_42E8_B393_EEBD12227F6A

#include "env.hpp"

namespace llvm {
class BasicBlock;
class Function;
class Instruction;
class raw_ostream;
class StringRef;
}  // namespace llvm

namespace stacksafe {

struct LogBlock {
  const Env prev, next;
  const llvm::BasicBlock* const block;

 public:
  LogBlock(const Env& p, const llvm::BasicBlock* b, const Env& n);
  void print(llvm::raw_ostream& os) const;
};

struct Log {
  const llvm::Function* const function;
  std::string filename;
  std::vector<LogBlock> blocks;

 public:
  explicit Log(const llvm::Function& func);
  llvm::StringRef get_filename() const;
  void add(const Env& prev, const llvm::BasicBlock* block, const Env& next);
  void print(llvm::raw_ostream& os) const;
};

void unknown_instruction(llvm::Instruction& i);
void check_interpret(llvm::Instruction& i, bool success);

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_91A166AC_CDE2_42E8_B393_EEBD12227F6A
