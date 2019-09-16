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

Log::Log(const llvm::Function& func)
    : function{&func}, filename{"log/" + func.getName().str() + ".log"} {}
void Log::add(const llvm::BasicBlock* block, const Env& prev, const Env& next) {
  blocks.emplace_back(block, prev, next);
}
void Log::print(llvm::raw_ostream& os) const {
  const auto hr = "--------------------------------";
  llvm::StringRef name{filename};
  std::error_code error;
  llvm::raw_fd_ostream file{name, error};
  if (error) {
    endline(llvm::errs() << "Error: " << error.message());
  } else {
    endline(os << "Print to: " << name);
  }
  llvm::raw_ostream& stream = error ? llvm::outs() : file;
  stream << *function;
  for (auto& log : blocks) {
    endline(stream);
    endline(stream << hr);
    log.print(stream);
  }
}

}  // namespace stacksafe
