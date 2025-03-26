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
            // errs() << "collect target allocas for " << F.getName() << "\n";
            for (BasicBlock& BB: F) {
                for (Instruction& inst: BB) {
                    if (isa<AllocaInst>(inst)) {
                        auto* allocaInst = cast<AllocaInst>(&inst);
                        if (std::all_of(inst.users().begin(), inst.users().end(),
                            [](User *u) { return isa<LoadInst>(u) || isa<StoreInst>(u); })) {
                            TargetAllocas.emplace(allocaInst);
                        }
                    }
                }
            }
            // errs() << "finished collect target allocas for " << F.getName() << "\n";
        }

        void removeInstructions(std::vector<Instruction*> &toRemove, Function &F) {
            // errs() << "remove instructions for " << F.getName() << "\n";
            for (auto inst: toRemove) {
                assert(inst != nullptr);
                inst->eraseFromParent();
                if (auto allocaInst = dyn_cast<AllocaInst>(inst)) {
                    // for all BB, delete from Post and Pre
                    for (BasicBlock &BB: F) {
                        Post[&BB].erase(allocaInst);
                        Pre[&BB].erase(allocaInst);
                        TargetAllocas.erase(allocaInst);
                    }
                }
            }
            toRemove.clear();
            // errs() << "finished remove instructions for " << F.getName() << "\n";
        }

        template<typename T>
        void accumulateUnusedInstructions(Function &F, std::vector<Instruction*> &toRemove) {
            // errs() << "accumulating unused instructions for " << F.getName() << "\n";
            for (BasicBlock& BB: F) {
                for (Instruction& inst: BB) {
                    // for store, there may not be users, but we don't want to remove it unless it's using alloca
                    if (isa<StoreInst>(inst)) {
                        if (TargetAllocas.find(dyn_cast<AllocaInst>(getPointerOperand(&inst))) != TargetAllocas.end()) {
                            toRemove.emplace_back(&inst);
                        }
                    } else {
                        if (isa<T>(inst) && inst.user_empty()) {
                            // errs() << "Removing instruction " << inst << "!\n";
                            toRemove.push_back(&inst);
                        }
                    }
                }
            }
            // errs() << "finished accumulating unused instructions for " << F.getName() << "\n";
        }

        void addPhiNodes(Function &F) {
            // errs() << "add phi nodes for " << F.getName() << "\n";
            for (BasicBlock& BB: F) {
                // don't add phi nodes if there are no predecessors
                if (&F.getEntryBlock() == &BB || predecessors(&BB).empty()) {
                    // errs() << "Found entry block " << BB.getName() << "!\n";
                    continue;
                }
                for (AllocaInst* allocaInst : TargetAllocas) {
                    assert(allocaInst);
                    if (!Pre[&BB][allocaInst]) {
                        auto name = allocaInst->getName() + "_" + BB.getName();
                        // this could be wrong, wha tis the correct type? Right now every phi node has i32
                        PHINode* phiNode = PHINode::Create(allocaInst->getAllocatedType(), pred_size(&BB), name, &BB.front());
                        Pre[&BB][allocaInst] = phiNode;
                        Post[&BB][allocaInst] = phiNode;
                    }
                }
            }
            // errs() << "finished add phi nodes for " << F.getName() << "\n";
        }

        void replaceLoads(Function &F) {
            // errs() << "replace loads for " << F.getName() << "\n";
            for (BasicBlock& BB: F) {
                for (Instruction& inst: BB) {
                    if (auto storeInst = dyn_cast<StoreInst>(&inst)) {
                        auto allocaInst = dyn_cast<AllocaInst>(storeInst->getPointerOperand());
                        if (TargetAllocas.count(allocaInst)) {
                            Post[&BB][allocaInst] = storeInst->getValueOperand();
                        }
                    } else if (auto loadInst = dyn_cast<LoadInst>(&inst)) {
                        auto allocaInst = dyn_cast<AllocaInst>(loadInst->getPointerOperand());
                        if (TargetAllocas.count(allocaInst)) {
                            loadInst->replaceAllUsesWith(Post[&BB][allocaInst]);
                        }
                    }
                }
            }
            // errs() << "finished replace loads for " << F.getName() << "\n";
        }

        void addPhiNodeEdges(Function &F) {
            // errs() << "add phi node edges for " << F.getName() << "\n";
            for (const auto& entry: Pre) {
                for (auto innerEntry: entry.second) {
                    auto allocaInst = dyn_cast<AllocaInst>(innerEntry.first);
                    auto predBBs = predecessors(entry.first);
                    auto phiNode = innerEntry.second;
                    assert(phiNode);
                    for (auto predBB: predBBs) {
                        if (Post[predBB][allocaInst]) {
                            assert(Post[predBB][allocaInst]);
                            assert(predBB);
                            assert(phiNode);
                            // errs() << "PhiNode " << phiNode->getName() << " already exists\n";
                            // phiNode->getType()->print(errs());
                            // Post[predBB][allocaInst]->print(errs());
                            phiNode->addIncoming(Post[predBB][allocaInst], predBB);
                        } else {
                            // errs() << "PhiNode " << phiNode->getName() << "has no post value\n";
                            // phiNode->getType()->print(errs());
                            // Post[predBB][allocaInst]->print(errs());
                            phiNode->addIncoming(UndefValue::get(allocaInst->getAllocatedType()), predBB);
                        }
                    }
                }
            }
            // errs() << "finished add phi node edges for " << F.getName() << "\n";
        }

        void removeUnusedInstructions(Function &F) {
            std::vector<Instruction*> toRemove;
            accumulateUnusedInstructions<PHINode>(F, toRemove);
            removeInstructions(toRemove, F);
            accumulateUnusedInstructions<LoadInst>(F, toRemove);
            removeInstructions(toRemove, F);
            accumulateUnusedInstructions<StoreInst>(F, toRemove);
            removeInstructions(toRemove, F);
            accumulateUnusedInstructions<AllocaInst>(F, toRemove);
            removeInstructions(toRemove, F);
        }

        virtual bool runOnFunction(Function &F) {
            // errs() << "Working on function called " << F.getName() << "!\n";
            collectTargetAllocas(F);
            addPhiNodes(F);
            replaceLoads(F);
            addPhiNodeEdges(F);
            removeUnusedInstructions(F);
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