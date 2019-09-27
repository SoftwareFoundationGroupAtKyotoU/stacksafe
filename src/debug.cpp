#include "debug.hpp"
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Function.h>
#include "fabric.hpp"
#include "json.hpp"
#include "utility.hpp"

namespace stacksafe {

LogFile::LogFile(const std::string& name) {
  std::error_code error;
  file = std::make_unique<llvm::raw_fd_ostream>(name, error);
  if (error) {
    endline(llvm::errs() << "Error: " << error.message());
  }
}
llvm::raw_ostream& LogFile::get() const { return file ? *file : llvm::errs(); }

std::string Log::logfilename(const llvm::Function& f) {
  return "log/" + f.getName().str() + ".log";
}
void Log::print_block(llvm::raw_ostream& os, const llvm::BasicBlock* block,
                      const Memory& prev, const Memory& next) {
  Fabric memory;
  {
    Fabric left, right;
    left.append("heap [prev]:").next().append(dump(prev.heap()));
    right.append("heap [next]:").next().append(dump(next.heap()));
    memory.append(left.patch(right.indent(2))).next();
  }
  {
    Fabric left, right;
    left.append("stack [prev]:").next().append(dump(prev.stack()));
    right.append("stack [next]:").next().append(dump(next.stack()));
    memory.append(left.patch(right.indent(2))).next();
  }
  endline(os << *block << memory);
}
Log::Log(const llvm::Function& func) : file{logfilename(func)} {
  file.get() << func;
}
void Log::print(const llvm::BasicBlock* block, const Memory& prev,
                const Memory& next) const {
  const auto hr = "--------------------------------";
  auto& os = file.get();
  os << hr;
  print_block(os, block, prev, next);
}

}  // namespace stacksafe
