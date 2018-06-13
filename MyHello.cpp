//===- MyHello.cpp - Example code from "Writing an LLVM Pass" ---------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements two versions of the LLVM "MyHello World" pass described
// in docs/WritingAnLLVMPass.html
//
//===----------------------------------------------------------------------===//

#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

#define DEBUG_TYPE "myhello"

STATISTIC(MyHelloCounter, "Counts number of functions greeted");

namespace {
  // MyHello - The first implementation, without getAnalysisUsage.
  struct MyHello : public FunctionPass {
    static char ID; // Pass identification, replacement for typeid
    MyHello() : FunctionPass(ID) {}

    bool runOnFunction(Function &F) override {
      ++MyHelloCounter;
      errs() << "MyHello: ";
      errs().write_escaped(F.getName()) << '\n';
      return false;
    }
  };
}

char MyHello::ID = 0;
static RegisterPass<MyHello> X("myhello", "MyHello World Pass", false, true);

namespace {
  // MyHello2 - The second implementation with getAnalysisUsage implemented.
  struct MyHello2 : public FunctionPass {
    static char ID; // Pass identification, replacement for typeid
    MyHello2() : FunctionPass(ID) {}

    bool runOnFunction(Function &F) override {
      ++MyHelloCounter;
      errs() << "MyHello: ";
      errs().write_escaped(F.getName()) << '\n';
      return false;
    }

    // We don't modify the program, so we preserve all analyses.
    void getAnalysisUsage(AnalysisUsage &AU) const override {
      AU.setPreservesAll();
    }
  };
}

char MyHello2::ID = 0;
static RegisterPass<MyHello2>
Y("myhello2", "MyHello World Pass (with getAnalysisUsage implemented)");
