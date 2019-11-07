#include "log.hpp"
#include <llvm/IR/Function.h>
#include <llvm/Support/Debug.h>
#include <llvm/Support/raw_ostream.h>

#define STACKSAFE_DEBUG_LOG(x) DEBUG_WITH_TYPE("log", x)

namespace stacksafe {
namespace {
std::string logfilename(const llvm::Function &f) {
  return "log/" + f.getName().str() + ".log";
}
void endline(llvm::raw_ostream &os) {
  (os << "\n").flush();
}
}  // namespace

LogFile::LogFile(const std::string &name) {
  std::error_code error;
  STACKSAFE_DEBUG_LOG(
      { file = std::make_unique<llvm::raw_fd_ostream>(name, error); });
  if (error) {
    endline(llvm::errs() << "Error: " << error.message());
  }
}
LogFile::~LogFile() = default;
llvm::raw_ostream &LogFile::get() const {
  return file ? *file : llvm::errs();
}

Log::Log(const llvm::Function &func) : file{logfilename(func)}, os{nullptr} {
  STACKSAFE_DEBUG_LOG(os = &file.get());
  if (os) {
    *os << func;
  }
}
const Log &Log::print(const std::string &s) const {
  if (os) {
    *os << s;
  }
  return *this;
}
const Log &Log::print(const llvm::Instruction &i) const {
  if (os) {
    endline(*os << i);
  }
  return *this;
}
const Log &Log::print(const llvm::BasicBlock &b) const {
  static const auto hr2 = "================================";
  static const auto hr = "--------------------------------";
  if (os) {
    endline(*os << hr2 << b << hr);
  }
  return *this;
}
const Log &Log::print(const Domain &d) const {
  if (os) {
    endline(*os << cache_.to_str(d));
  }
  return *this;
}
const Log &Log::print(const Symbol &key, const Domain &val) const {
  if (os) {
    endline(*os << cache_.to_str(key) << ": " << cache_.to_str(val));
  }
  return *this;
}
const Log &Log::print(const Register &key, const Domain &val) const {
  if (os) {
    endline(*os << cache_.to_str(key) << ": " << cache_.to_str(val));
  }
  return *this;
}

}  // namespace stacksafe
