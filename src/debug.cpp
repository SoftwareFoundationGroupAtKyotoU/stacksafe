#include "debug.hpp"
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instruction.h>
#include <llvm/Support/raw_ostream.h>
#include "fabric.hpp"
#include "json.hpp"

namespace stacksafe {

LogBlock::LogBlock(const llvm::BasicBlock* b, const Env& p, const Env& n)
    : block{b}, prev{p}, next{n} {}
void LogBlock::print(llvm::raw_ostream& os) const {
  Fabric env;
  {
    Fabric left, right;
    left.append("heap: [prev]").next().append(dump(prev.heap()));
    right.append("heap: [next]").next().append(dump(next.heap()));
    env.append(left.patch(right.indent(2))).next();
  }
  {
    Fabric left, right;
    left.append("stack: [prev]").next().append(dump(prev.stack()));
    right.append("stack: [next]").next().append(dump(next.stack()));
    env.append(left.patch(right.indent(2))).next();
  }
  os << *block << "\n" << env;
}

Log::Log(const llvm::Function& func)
    : function{&func}, filename{"log/" + func.getName().str()} {}
void Log::add(const llvm::BasicBlock* block, const Env& prev, const Env& next) {
  blocks.emplace_back(block, prev, next);
}
void Log::print(llvm::raw_ostream& os) const {
  const auto hr = "--------------------------------";
  llvm::StringRef name{filename};
  std::error_code error;
  llvm::raw_fd_ostream file{name, error};
  if (error) {
    llvm::errs() << "Error: " << error.message() << "\n";
  } else {
    os << "Print to: " << name << "\n";
  }
  llvm::raw_ostream& stream = error ? llvm::outs() : file;
  stream << *function;
  for (auto& log : blocks) {
    log.print(stream);
    stream << "\n" << hr << "\n";
  }
}

void unknown_instruction(llvm::Instruction& i) {
  llvm::errs() << "unknown: " << i << "\n";
}
void check_interpret(llvm::Instruction& i, bool success) {
  if (!success) {
    llvm::errs() << "Failed to interpret: " << i << "\n";
  }
}
void missing_log(llvm::raw_ostream& os) { os << "Error: missing log object\n"; }

}  // namespace stacksafe
