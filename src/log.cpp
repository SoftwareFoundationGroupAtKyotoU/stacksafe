#include "log.hpp"
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Function.h>
#include <llvm/Support/raw_ostream.h>
#include "fabric.hpp"
#include "json.hpp"
#include "memory.hpp"
#include "utility.hpp"

namespace stacksafe {

LogFile::LogFile(const std::string &name) {
  std::error_code error;
  file = std::make_unique<llvm::raw_fd_ostream>(name, error);
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
Log::Log(const llvm::Function &func) : file{logfilename(func)} {}
void Log::print_func(const llvm::Function &f) const {
  const auto hr = "================================";
  file.get() << hr << f;
}
void Log::print_diff(const llvm::BasicBlock &block, const Fabric &fab) const {
  const auto hr = "--------------------------------";
  auto &f = file.get();
  endline(f << hr << block);
  endline(f << fab);
}
void Log::print_env(const llvm::BasicBlock &block, const Memory &mem) const {
  const auto hr = "--------------------------------";
  auto &f = file.get();
  Json j = mem;
  endline(f << hr);
  endline(f << j.dump(2) << block);
}

}  // namespace stacksafe
