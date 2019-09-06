#include "debug.hpp"
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instruction.h>
#include <llvm/Support/raw_ostream.h>
#include "json.hpp"

namespace stacksafe {

LogBlock::LogBlock(const Env& p, const llvm::BasicBlock* b, const Env& n)
    : prev{p}, next{n}, block{b} {}
void LogBlock::print() const {
  const auto hr = "----------------", endl = "\n";
  Json p = prev, n = next;
  llvm::errs() << hr << endl;
  llvm::errs() << p.dump(0) << endl;
  llvm::errs() << *block;
  llvm::errs() << n.dump(0) << endl;
  llvm::errs() << hr << endl;
}

Log::Log(const llvm::Function& func) : function{&func} {}
void Log::add(const Env& prev, const llvm::BasicBlock* block, const Env& next) {
  blocks.emplace_back(prev, block, next);
}
void Log::print() const {
  llvm::errs() << *function;
  for (auto& log : blocks) {
    log.print();
  }
}

void unknown_instruction(llvm::Instruction& i) {
  llvm::errs() << "unknown: " << i << "\n";
}

}  // namespace stacksafe
