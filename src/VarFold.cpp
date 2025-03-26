// Simple variable folding
// A variable can be substituted into following usage if:
// - the variable is not modified between the following usage
// - the variable is a constant value
// After folding, need to remove the load instruction for that variable

//
// Created by ubuntu on 2025/3/25.
//

#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/IR/CFG.h"
#include <set>
#include <llvm-15/llvm/IR/Constants.h>

using namespace llvm;

namespace {
    struct VarFold : public FunctionPass {
        static char ID;
        VarFold() : FunctionPass(ID) {}

        //declare more varibles if you like
        std::set<AllocaInst*> ConstantVars;

        void collectConstantVars(Function &F) {
            for (BasicBlock& BB: F) {
                for (Instruction& inst: BB) {
                    if (auto* loadInst = dyn_cast<LoadInst>(&inst)) {
                        if (auto* constant = dyn_cast<Constant>(loadInst->getOperand(0))) {
                            // need to find instructions using it between loadInst and the next store inst to this variable
                            // we could replace all occurrences after,a then at the store replace all occurrences after again
                            // to ensure we did not use stale value
                        }
                    }
                }
            }
            // errs() << "finished collect target allocas for " << F.getName() << "\n";
        }

        virtual bool runOnFunction(Function &F) {
            collectConstantVars(F);
            return true;
        }
    };
}

char VarFold::ID = 1;

static RegisterPass<VarFold> X("varFold", "Variable Folding pass for minic", false, false);

// Automatically enable the pass.
// http://adriansampson.net/blog/clangpass.html
static void registerVarFold(const PassManagerBuilder &,
                                    legacy::PassManagerBase &PM) {
    PM.add(new VarFold());
}
static RegisterStandardPasses
        RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible,
                       registerVarFold);