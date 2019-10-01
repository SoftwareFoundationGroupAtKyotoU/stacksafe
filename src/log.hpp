#ifndef INCLUDE_GUARD_91A166AC_CDE2_42E8_B393_EEBD12227F6A
#define INCLUDE_GUARD_91A166AC_CDE2_42E8_B393_EEBD12227F6A

#include <memory>

namespace llvm {
class BasicBlock;
class Function;
class raw_fd_ostream;
class raw_ostream;
}  // namespace llvm

namespace stacksafe {
class Memory;

struct LogFile {
  std::unique_ptr<llvm::raw_fd_ostream> file;

 public:
  explicit LogFile(const std::string& name);
  ~LogFile();
  llvm::raw_ostream& get() const;
};

struct Log {
  LogFile file;
  static std::string logfilename(const llvm::Function& f);
  static void print_block(llvm::raw_ostream& os, const llvm::BasicBlock* block,
                          const Memory& prev, const Memory& next);

 public:
  explicit Log(const llvm::Function& func);
  void print(const llvm::Function& f) const;
  void print(const llvm::BasicBlock* block, const Memory& prev,
             const Memory& next) const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_91A166AC_CDE2_42E8_B393_EEBD12227F6A
