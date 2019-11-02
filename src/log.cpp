#include "log.hpp"
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Function.h>
#include <llvm/Support/Debug.h>
#include <llvm/Support/raw_ostream.h>
#include "fabric.hpp"
#include "json.hpp"
#include "memory.hpp"
#include "utility.hpp"

#define STACKSAFE_DEBUG_LOG(x) DEBUG_WITH_TYPE("log", x)

namespace stacksafe {

LogFile::LogFile(const std::string &name) {
  std::error_code error;
  STACKSAFE_DEBUG_LOG(file =
                          std::make_unique<llvm::raw_fd_ostream>(name, error));

  if (error) {
    endline(llvm::errs() << "Error: " << error.message());
  }
}
LogFile::~LogFile() = default;
llvm::raw_ostream &LogFile::get() const {
  return file ? *file : llvm::errs();
}

std::string Log::logfilename(const llvm::Function &f) {
  return "log/" + f.getName().str() + ".log";
}
Log::Log(const llvm::Function &func) : file{logfilename(func)}, os{nullptr} {
  STACKSAFE_DEBUG_LOG(os = &file.get());
}
void Log::print(const llvm::Value &v) const {
  if (os) {
    *os << v;
  }
}
void Log::print(const Register &key, const Domain &val) const {
  if (os) {
    Fabric fab;
    fab.append(dump(key)).append(": ").append(dump(val));
    endline(*os << fab);
  }
}
void Log::print(const Symbol &key, const Domain &val) const {
  if (os) {
    Fabric fab;
    fab.append(dump(key)).append(": ").append(dump(val));
    endline(*os << fab);
  }
}
void Log::print_hr() const {
  const auto hr = "--------------------------------";
  file.get() << hr;
}
void Log::print_inst(const llvm::Instruction &i) const {
  endline(file.get() << to_str(i));
}

}  // namespace stacksafe
