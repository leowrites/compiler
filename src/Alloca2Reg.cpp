//
// Created by Fan Long on 2020/12/9.
//

//add more header files if your want
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/IR/CFG.h"
#include <set>
#include <map>
#include <llvm-15/llvm/IR/Constants.h>

using namespace llvm;

namespace {
    struct Alloca2RegPass : public FunctionPass {
        static char ID;
        Alloca2RegPass() : FunctionPass(ID) {}

        //declare more varibles if you like
        std::set<AllocaInst*> TargetAllocas;
        std::map<BasicBlock*, std::map<AllocaInst*, Value*> > Post;   // value at the end of the BB, could be store value or a phi node
        std::map<BasicBlock*, std::map<AllocaInst*, PHINode*> > Pre;  // value before the BB

        void collectTargetAllocas(Function &F) {
           //start your code here
            // for each alloca instruction, check who is using it
            // if all users are loads and stores, then we add it to TargetAllocas
            for (BasicBlock& BB: F) {
                for (Instruction& inst: BB) {
                    if (isa<AllocaInst>(inst)) {
                        AllocaInst* allocaInst = cast<AllocaInst>(&inst);
                        if (std::all_of(inst.users().begin(), inst.users().end(),
                            [](User *u) { return isa<LoadInst>(u) || isa<StoreInst>(u); })) {
                            TargetAllocas.emplace(allocaInst);
                        }
                    }
                }
            }
        }

        void removeInstructions(std::vector<Instruction*> &toRemove) {
            for (auto inst: toRemove) {
                assert(inst != nullptr);
                inst->eraseFromParent();
            }
            toRemove.clear();
        }

        template<typename T>
        void accumulateUnusedInstructions(Function &F, std::vector<Instruction*> &toRemove) {
            for (BasicBlock& BB: F) {
                for (Instruction& inst: BB) {
                    if (isa<T>(inst) && inst.user_empty()) {
                        errs() << "Removing instruction " << inst << "!\n";
                        toRemove.push_back(&inst);
                    }
                }
            }
        }

        virtual bool runOnFunction(Function &F) {
            errs() << "Working on function called " << F.getName() << "!\n";
            collectTargetAllocas(F);

            // TODO: for each alloca, create PHI in all basic blocks of F
            for (BasicBlock& BB: F) {
                if (&F.getEntryBlock() == &BB) {
                    errs() << "Found entry block " << BB.getName() << "!\n";
                    continue;
                }
                for (AllocaInst* allocaInst : TargetAllocas) {
                    if (!Pre[&BB][allocaInst]) {
                        auto name = allocaInst->getName() + "_" + BB.getName();
                        PHINode* phiNode = PHINode::Create(allocaInst->getOperand(0)->getType(), pred_size(&BB), name, &BB.front());
                        Pre[&BB][allocaInst] = phiNode;
                        Post[&BB][allocaInst] = phiNode;
                    }
                }
            }

            for (BasicBlock& BB: F) {
                for (Instruction& inst: BB) {
                    if (auto storeInst = dyn_cast<StoreInst>(&inst)) {
                        auto allocaInst = dyn_cast<AllocaInst>(storeInst->getPointerOperand());
                        if (TargetAllocas.count(allocaInst)) {
                            Post[&BB][allocaInst] = storeInst->getValueOperand();
                        }
                    } else if (auto loadInst = dyn_cast<LoadInst>(&inst)) {
                        // for load instructions, create PHI node if no mapping in Post,
                        // otherwise replace itself with the value in Post
                        auto allocaInst = dyn_cast<AllocaInst>(loadInst->getPointerOperand());
                        if (TargetAllocas.count(allocaInst)) {
                            loadInst->replaceAllUsesWith(Post[&BB][allocaInst]);
                        }
                    }
                }
            }
            for (const auto& entry: Pre) {
                for (auto innerEntry: entry.second) {
                    auto allocaInst = dyn_cast<AllocaInst>(innerEntry.first);
                    auto predBBs = predecessors(entry.first);
                    auto phiNode = innerEntry.second;
                    if (predBBs.empty()) {
                        phiNode->setIncomingValue(0, UndefValue::get(allocaInst->getAllocatedType()));
                    }
                    for (auto predBB: predBBs) {
                        if (!Post[predBB][allocaInst]) {
                        } else {
                            phiNode->addIncoming(Post[predBB][allocaInst], predBB);
                        }
                    }
                }
            }

            // Remove instructions
            std::vector<Instruction*> toRemove;
            accumulateUnusedInstructions<PHINode>(F, toRemove);
            removeInstructions(toRemove);
            accumulateUnusedInstructions<LoadInst>(F, toRemove);
            removeInstructions(toRemove);
            accumulateUnusedInstructions<StoreInst>(F, toRemove);
            removeInstructions(toRemove);
            accumulateUnusedInstructions<AllocaInst>(F, toRemove);
            removeInstructions(toRemove);

            F.print(errs());
            return true;
        }
    };
}

char Alloca2RegPass::ID = 0;

static RegisterPass<Alloca2RegPass> X("alloca2reg", "Alloca-to-register pass for minic", false, false);

// Automatically enable the pass.
// http://adriansampson.net/blog/clangpass.html
static void registerAlloca2RegPass(const PassManagerBuilder &,
                                    legacy::PassManagerBase &PM) {
    PM.add(new Alloca2RegPass());
}
// static RegisterStandardPasses
//         RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible,
//                        registerAlloca2RegPass);