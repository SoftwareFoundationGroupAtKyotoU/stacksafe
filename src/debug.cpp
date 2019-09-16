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

LogBlock::LogBlock(const llvm::BasicBlock* b, const Env& p, const Env& n)
    : block{b}, prev{p}, next{n} {}
void LogBlock::print(llvm::raw_ostream& os) const {
  Fabric env;
  {
    Fabric left, right;
    left.append("heap [prev]:").next().append(dump(prev.heap()));
    right.append("heap [next]:").next().append(dump(next.heap()));
    env.append(left.patch(right.indent(2))).next();
  }
  {
    Fabric left, right;
    left.append("stack [prev]:").next().append(dump(prev.stack()));
    right.append("stack [next]:").next().append(dump(next.stack()));
    env.append(left.patch(right.indent(2))).next();
  }
  endline(os << *block);
  endline(os << env);
}

std::string Log::logfilename(const llvm::Function& f) {
  return "log/" + f.getName().str() + ".log";
}
Log::Log(const llvm::Function& func)
    : function{&func},
      filename{"log/" + func.getName().str() + ".log"},
      file{logfilename(func)} {}
void Log::add(const llvm::BasicBlock* block, const Env& prev, const Env& next) {
  blocks.emplace_back(block, prev, next);
}
void Log::print(llvm::raw_ostream&) const {
  const auto hr = "--------------------------------";
  llvm::raw_ostream& stream = file.get();
  stream << *function;
  for (auto& log : blocks) {
    endline(stream);
    endline(stream << hr);
    log.print(stream);
  }
}

}  // namespace stacksafe
