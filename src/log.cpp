#include "log.hpp"
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Function.h>
#include <llvm/Support/raw_ostream.h>
#include "fabric.hpp"
#include "json.hpp"
#include "memory.hpp"
#include "utility.hpp"

namespace stacksafe {

LogFile::LogFile(const std::string& name) {
  std::error_code error;
  file = std::make_unique<llvm::raw_fd_ostream>(name, error);
  if (error) {
    endline(llvm::errs() << "Error: " << error.message());
  }
}
LogFile::~LogFile() = default;
llvm::raw_ostream& LogFile::get() const {
  return file ? *file : llvm::errs();
}

std::string Log::logfilename(const llvm::Function& f) {
  return "log/" + f.getName().str() + ".log";
}
void Log::print_block(llvm::raw_ostream& os, const llvm::BasicBlock* block,
                      const Memory& prev, const Memory& next) {
  auto patch = [](const std::string& tag, const auto& p, const auto& n) {
    Fabric left, right;
    left.append(tag + " [prev]:").next().append(dump(p));
    right.append(tag + " [next]:").next().append(dump(n));
    return left.patch(right.indent(2));
  };
  Fabric memory;
  memory.append(patch("heap", prev.heap(), next.heap())).next();
  memory.append(patch("stack", prev.stack(), next.stack())).next();
  endline(os << *block << memory);
}
Log::Log(const llvm::Function& func) : file{logfilename(func)} {}
void Log::print(const llvm::Function& f) const {
  file.get() << f;
}
void Log::print(const llvm::BasicBlock* block, const Memory& prev,
                const Memory& next) const {
  const auto hr = "--------------------------------";
  print_block((file.get() << hr), block, prev, next);
}

}  // namespace stacksafe
