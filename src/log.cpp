#include "log.hpp"
#include <llvm/IR/Function.h>
#include <llvm/Support/Debug.h>
#include <llvm/Support/raw_ostream.h>
#include "domain.hpp"
#include "error.hpp"

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

Log::Log(const llvm::Function &func)
    : cache_{func}, file{logfilename(func)}, os{nullptr} {
  STACKSAFE_DEBUG_LOG(os = &file.get());
  if (os) {
    *os << func;
  }
}
void Log::print(const llvm::Instruction &i) const {
  if (os) {
    endline(*os << i);
  }
}
void Log::print(const llvm::BasicBlock &b) const {
  static const auto hr2 = "================================";
  static const auto hr = "--------------------------------";
  if (os) {
    endline(*os << hr2 << b << hr);
  }
}
void Log::print_heap(const Symbol &key, const Domain &val,
                     const Domain &add) const {
  if (os) {
    endline(*os << "&" << cache_.to_str(key.value()) << ": "
                << cache_.to_str(val) << " += " << cache_.to_str(add));
  }
}
void Log::print_stack(const Value &key, const Domain &val,
                      const Domain &add) const {
  if (os) {
    endline(*os << "%" << cache_.to_str(key) << ": " << cache_.to_str(val)
                << " += " << cache_.to_str(add));
  }
}
void Log::print(const Error &err) const {
  if (os) {
    err.print(*os);
  }
}

}  // namespace stacksafe
