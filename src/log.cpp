#include "log.hpp"
#include <llvm/IR/Function.h>
#include <llvm/IR/Value.h>
#include <llvm/Support/Debug.h>
#include <llvm/Support/raw_ostream.h>
#include "domain.hpp"
#include "fabric.hpp"
#include "register.hpp"
#include "utility.hpp"

#define STACKSAFE_DEBUG_LOG(x) DEBUG_WITH_TYPE("log", x)

namespace stacksafe {
namespace {
std::string logfilename(const llvm::Function &f) {
  return "log/" + f.getName().str() + ".log";
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
}
const Log &Log::print(const std::string &s) const {
  if (os) {
    endline(*os << s);
  }
  return *this;
}
const Log &Log::print(const llvm::Value &v) const {
  if (os) {
    *os << v;
  }
  return *this;
}
const Log &Log::print(const Register &key, const Domain &val) const {
  if (os) {
    Fabric fab;
    fab.append(to_str(key)).append(": ").append(dump(val));
    endline(*os << fab);
  }
  return *this;
}
const Log &Log::print(const llvm::Value &key, const Domain &val) const {
  if (os) {
    endline(*os << get_operand(key) << ": " << dump(val));
  }
  return *this;
}
const Log &Log::print_hr() const {
  if (os) {
    const auto hr = "--------------------------------";
    *os << hr;
  }
  return *this;
}
const Log &Log::print_hr2() const {
  if (os) {
    const auto hr = "================================";
    *os << hr;
  }
  return *this;
}
const Log &Log::print_nl() const {
  if (os) {
    endline(*os);
  }
  return *this;
}
void Log::error_global(const Domain &dom) const {
  if (os) {
    endline(*os << "ERROR[GLOBAL]: " << dump(dom));
  }
}
void Log::error_call(const Domain &v) const {
  if (os) {
    endline(*os << "ERROR[CALL]: " << dump(v));
  }
}
void Log::error_return(const Domain &v) const {
  if (os) {
    endline(*os << "ERROR[RETURN]: " << dump(v));
  }
}

}  // namespace stacksafe
