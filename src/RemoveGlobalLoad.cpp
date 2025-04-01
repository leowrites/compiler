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
#include "llvm/Transforms/Scalar/LICM.h"
#include "llvm/IR/IRBuilder.h"

using namespace llvm;

namespace
{
    struct RemoveGlobalLoad : public FunctionPass
    {
        static char ID;
        RemoveGlobalLoad() : FunctionPass(ID) {}

        std::map<GlobalVariable *, Value *> lastLoadedValues;
        std::vector<GlobalVariable *> varToHoist;
        std::map<GlobalVariable *, LoadInst *> loadInstToHoist;

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

        void getHoistableGlobals(Loop &L)
        {
            for (BasicBlock *BB : L.blocks())
            {
                // within the block, check next instructions, only do the simple case so
                // stop if there are call or branch and store
                // after all instructions in the block, we add it to varToHoist
                for (auto &I : *BB)
                    if (auto *loadInst = dyn_cast<LoadInst>(&I))
                    {
                        if (!isa<GlobalVariable>(loadInst->getPointerOperand()))
                        {
                            continue;
                        }
                        // errs () << *loadInst << '\n';
                        bool canHoist = true;
                        for (auto J = std::next(BasicBlock::iterator(loadInst)); J != BB->end();)
                        {
                            Instruction *nextInst = &*J;
                            ++J;
                            if (isa<CallInst>(nextInst))
                            {
                                canHoist = false;
                                break;
                            }
                            // if the store inst uses the load inst, then break
                            if (auto storeInst = dyn_cast<StoreInst>(nextInst))
                            {
                                if (storeInst->getPointerOperand() == loadInst->getPointerOperand())
                                {
                                    // errs () << "    " << *storeInst << '\n';
                                    canHoist = false;
                                    break;
                                }
                            }
                        }
                        if (canHoist)
                        {
                            if (auto gv = dyn_cast<GlobalVariable>(loadInst->getPointerOperand()))
                            {
                                // errs() << *loadInst << "\n";
                                varToHoist.emplace_back(gv);
                                loadInstToHoist[gv] = loadInst;
                            }
                        }
                    }
            }
        }

        void getAnalysisUsage(AnalysisUsage &info) const
        {
            info.setPreservesCFG();
            // Get analysis pass on loops so we can hoist global vars
            info.addRequired<LoopInfoWrapperPass>();
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

            // remove global loads in a loop
            LoopInfo &LI = getAnalysis<LoopInfoWrapperPass>().getLoopInfo();
            for (Loop *L : LI)
            {
                // this works if there is only one variable in the loop, if there are multiple,
                // need to allow if any of them is only loaded
                getHoistableGlobals(*L);
                // for each block in the loop
                // for each instructions
                // check if it's a load and if it's global
                // (the following part can be done in the previous step)
                // if true, check the rest of the blocks to see if it is stored to
                // if is not stored to, then we can get the loop header and move the load
                // to outside of the loop
                for (auto gv : varToHoist)
                {
                    // errs() << *gv << "\n";
                    // For each of the global vars, find the load instruction and move it to before
                    // the loop header
                    // errs() << *L->getLoopPreheader() << "\n";
                    // this should be the second to last instruction
                    Instruction *lastInstruction = L->getLoopPreheader()->getTerminator();
                    IRBuilder<> Builder(lastInstruction);
                    auto newLoad = Builder.CreateLoad(gv->getOperand(0)->getType(), gv);
                    // for each inst that used to use gv, we replace the uses with newLoad
                    for (auto it = loadInstToHoist.begin(); it != loadInstToHoist.end(); it++)
                    {
                        auto gvToReplace = it->first;
                        auto loadInst = it->second;
                        if (gvToReplace == gv)
                        {
                            loadInst->replaceAllUsesWith(newLoad);
                            loadInst->eraseFromParent();
                        }
                    }
                }
            }
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