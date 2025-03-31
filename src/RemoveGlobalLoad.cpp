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
#include "map"
#include <set>
#include <llvm-15/llvm/IR/Constants.h>
#include "llvm/Analysis/LoopInfo.h"


using namespace llvm;

namespace
{
    struct RemoveGlobalLoad : public FunctionPass
    {
        static char ID;
        RemoveGlobalLoad() : FunctionPass(ID) {}

        std::map<GlobalVariable *, Value *> lastLoadedValues;

        // If this is a global variable, check the next lines to see if there are more
        // load instructions, if there are, replace their values with the value of
        // the current load
        // While going through the next lines if there is a branch or a call, stop
        // because another scope could modify the global var
        void simpleGlobalLoadRemoval(BasicBlock &BB)
        {

            for (auto I = BB.begin(); I != BB.end(); I++)
            {
                // while looping, need to check if the instruction has been erased
                Instruction *inst = &*I;
                if (!inst->getParent())
                    continue;

                if (auto *loadInst = dyn_cast<LoadInst>(inst))
                {
                    if (auto *GV = dyn_cast<GlobalVariable>(loadInst->getPointerOperand()))
                    {
                        // errs() << GV-> << "\n";
                        lastLoadedValues[GV] = loadInst;

                        for (auto J = std::next(BasicBlock::iterator(inst)); J != BB.end();)
                        {
                            Instruction *nextInst = &*J;
                            ++J;

                            if (auto *nextLoadInst = dyn_cast<LoadInst>(nextInst))
                            {
                                if (dyn_cast<GlobalVariable>(nextLoadInst->getPointerOperand()) == GV)
                                {
                                    nextLoadInst->replaceAllUsesWith(loadInst);
                                    nextLoadInst->eraseFromParent();
                                }
                            }
                            else if (isa<CallInst>(nextInst) || isa<BranchInst>(nextInst))
                                break;
                        }
                    }
                }
            }
        }

        // This is a pass to remove global loads in the simple case, where multiple loads of the 
        // same global variable followed by each other with no jumping instruction in between is 
        // optimized out
        virtual bool runOnFunction(Function &F)
        {
            errs() << "Running RemoveGlobalLoad on function called " << F.getName() << "!\n";
            // First, remove loads in the simple case
            for (auto &BB : F)
                simpleGlobalLoadRemoval(BB);
            
            // Then, remove loads when inside a loop, but the loop does not modify the global variable
            // LoopInfo &LI = FAM.getResult<LoopAnalysis>(F);
            return true;
        }
    };
}

char RemoveGlobalLoad::ID = 0;

static RegisterPass<RemoveGlobalLoad> X("RemoveGlobalLoad", "Remove global loads pass for minic", false, false);

// Automatically enable the pass.
// http://adriansampson.net/blog/clangpass.html
static void registerVarFold(const PassManagerBuilder &,
                            legacy::PassManagerBase &PM)
{
    PM.add(new RemoveGlobalLoad());
}
static RegisterStandardPasses
    RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible,
                   registerVarFold);