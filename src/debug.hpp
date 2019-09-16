#ifndef INCLUDE_GUARD_91A166AC_CDE2_42E8_B393_EEBD12227F6A
#define INCLUDE_GUARD_91A166AC_CDE2_42E8_B393_EEBD12227F6A

#include <llvm/Support/raw_ostream.h>
#include <memory>
#include <vector>
#include "env.hpp"

namespace llvm {
class BasicBlock;
class Function;
}  // namespace llvm

namespace stacksafe {

struct LogFile {
  std::unique_ptr<llvm::raw_fd_ostream> file;

 public:
  explicit LogFile(const std::string& name);
  llvm::raw_ostream& get() const;
};

struct LogBlock {
  const llvm::BasicBlock* const block;
  const Env prev, next;

 public:
  LogBlock(const llvm::BasicBlock* b, const Env& p, const Env& n);
  void print(llvm::raw_ostream& os) const;
};

struct Log {
  std::vector<LogBlock> blocks;
  LogFile file;
  static std::string logfilename(const llvm::Function& f);

 public:
  explicit Log(const llvm::Function& func);
  void print(const llvm::BasicBlock* block, const Env& prev, const Env& next);
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_91A166AC_CDE2_42E8_B393_EEBD12227F6A
