#ifndef INCLUDE_GUARD_91A166AC_CDE2_42E8_B393_EEBD12227F6A
#define INCLUDE_GUARD_91A166AC_CDE2_42E8_B393_EEBD12227F6A

#include <memory>
#include "cache.hpp"

namespace llvm {
class BasicBlock;
class Function;
class Instruction;
class raw_fd_ostream;
class raw_ostream;
}  // namespace llvm

namespace stacksafe {
class Depend;
class Domain;
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
  const Cache cache_;
  const LogFile file;
  llvm::raw_ostream *os;

 public:
  explicit Log(const llvm::Function &func);
  explicit operator bool() const;
  bool print(const llvm::Instruction &i) const;
  bool print(const llvm::BasicBlock &b) const;
  bool print(const Symbol &key, const Domain &val, const Domain &add) const;
  bool print(const Register &key, const Domain &val, const Domain &add) const;
  bool print(const Depend &dep) const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_91A166AC_CDE2_42E8_B393_EEBD12227F6A
