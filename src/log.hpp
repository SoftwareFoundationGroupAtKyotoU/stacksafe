#ifndef INCLUDE_GUARD_91A166AC_CDE2_42E8_B393_EEBD12227F6A
#define INCLUDE_GUARD_91A166AC_CDE2_42E8_B393_EEBD12227F6A

#include <memory>

namespace llvm {
class BasicBlock;
class Function;
class Instruction;
class Value;
class raw_fd_ostream;
class raw_ostream;
}  // namespace llvm

namespace stacksafe {
class Domain;
class Fabric;
class Memory;
class Register;
class Symbol;

class LogFile {
  std::unique_ptr<llvm::raw_fd_ostream> file;

 public:
  explicit LogFile(const std::string &name);
  ~LogFile();
  llvm::raw_ostream &get() const;
};

class Log {
  LogFile file;
  llvm::raw_ostream *os;
  static std::string logfilename(const llvm::Function &f);

 public:
  explicit Log(const llvm::Function &func);
  void print(const llvm::Value &v) const;
  void print(const Register &key, const Domain &val) const;
  void print(const Symbol &key, const Domain &val) const;
  void print_hr() const;
  void print_nl() const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_91A166AC_CDE2_42E8_B393_EEBD12227F6A
