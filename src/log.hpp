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
class Domain;
class Error;
class Value;

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
  void print(const llvm::Instruction &i) const;
  void print(const llvm::BasicBlock &b) const;
  void print(const Value &key, const Domain &val, const Domain &add) const;
  void print(const llvm::Instruction &key, const Domain &val,
             const Domain &add) const;
  void print(const Error &err) const;
};

}  // namespace stacksafe

#endif  // INCLUDE_GUARD_91A166AC_CDE2_42E8_B393_EEBD12227F6A
