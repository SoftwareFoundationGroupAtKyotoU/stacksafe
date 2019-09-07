#include "debug.hpp"
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instruction.h>
#include <llvm/Support/raw_ostream.h>
#include "fabric.hpp"
#include "json.hpp"

namespace stacksafe {

LogBlock::LogBlock(const Env& p, const llvm::BasicBlock* b, const Env& n)
    : prev{p}, next{n}, block{b} {}
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

Log::Log(const llvm::Function& func) : function{&func} {}
void Log::add(const Env& prev, const llvm::BasicBlock* block, const Env& next) {
  blocks.emplace_back(prev, block, next);
}
void Log::print() const {
  const auto hr = "--------------------------------";
  llvm::errs() << *function;
  for (auto& log : blocks) {
    log.print(llvm::errs());
    llvm::errs() << "\n" << hr << "\n";
  }
}

void unknown_instruction(llvm::Instruction& i) {
  llvm::errs() << "unknown: " << i << "\n";
}

}  // namespace stacksafe
