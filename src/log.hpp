#ifndef INCLUDE_GUARD_91A166AC_CDE2_42E8_B393_EEBD12227F6A
#define INCLUDE_GUARD_91A166AC_CDE2_42E8_B393_EEBD12227F6A

#include <memory>

namespace llvm {
class BasicBlock;
class Function;
class Instruction;
class raw_fd_ostream;
class raw_ostream;
}  // namespace llvm

namespace stacksafe {
class Fabric;
class Memory;

class LogFile {
  std::unique_ptr<llvm::raw_fd_ostream> file;

 public:
  explicit LogFile(const std::string &name);
  ~LogFile();
  llvm::raw_ostream &get() const;
};

class Log {
  LogFile file;
  static std::string logfilename(const llvm::Function &f);

 public:
  explicit Log(const llvm::Function &func);
  void print_func(const llvm::Function &f) const;
  void print_diff(const llvm::BasicBlock &block, const Fabric &fab) const;
  void print_diff(const Fabric &fab) const;
  void print_env(const llvm::BasicBlock &block, const Memory &mem) const;
  void print_inst(const llvm::Instruction &i) const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_91A166AC_CDE2_42E8_B393_EEBD12227F6A
