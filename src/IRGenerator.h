//
// Created by Fan Long on 2020/12/6.
//

#ifndef MINICC_IRGENERATOR_H
#define MINICC_IRGENERATOR_H

//add more header files if your want
#include "ASTVisitor.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "Program.h"

namespace minicc {

    class IRGenerator : public ASTVisitor {
        std::unique_ptr<llvm::LLVMContext> TheContext;
        std::unique_ptr<llvm::Module> TheModule;
        std::unique_ptr<llvm::IRBuilder<>> TheBuilder;
        std::string ModuleName;

        std::map<Expr*, llvm::Value*> LLVMValueForExpr;
        std::vector<llvm::BasicBlock*> LoopExitStack;

        //add your variables and member functions
        llvm::Type* miniccTypeTollvmTypePrimitive(minicc::Type type) {
            if (type == Type(Type::Bool))
                return llvm::Type::getInt1Ty(*TheContext);
            if (type == Type(Type::Char))
                return llvm::Type::getInt8Ty(*TheContext);
            if (type == Type(Type::Int))
                return llvm::Type::getInt32Ty(*TheContext);
            if (type == Type(Type::Void))
                return llvm::Type::getVoidTy(*TheContext);
            llvm::errs() << "Unknown type: " << type.toString() << "\n"; // Add this
            return nullptr;
        };
        llvm::Type *miniccTypeTollvmType(minicc::Type type) {
            llvm::Type *llvmType = miniccTypeTollvmTypePrimitive(Type(type.primitiveType()));
            if (type.arrayBound() == 0)
            {
                return llvmType;
            }
            return llvm::ArrayType::get(llvmType, type.arrayBound());
        }

        // Code from LLVM tutorial, used to allocate on the stack
        llvm::AllocaInst *CreateEntryBlockAlloca(llvm::Function *function, std::string name,
                                                              llvm::Type *type, llvm::Value *arraySize)
        {
            assert(function);
            assert(type);
            llvm::IRBuilder<> TmpB(&function->getEntryBlock(), function->getEntryBlock().begin());
            return TmpB.CreateAlloca(type, arraySize, name);
        }

        void checkTerminatorAndCreateBr(llvm::BasicBlock* bb) {
            if (!TheBuilder->GetInsertBlock()->getTerminator())
                TheBuilder->CreateBr(bb);
        }

        llvm::Function *getFunctionBlock() {
            if (TheBuilder->GetInsertBlock()) {
                return TheBuilder->GetInsertBlock()->getParent();
            }
            return nullptr;
        }
    public:
        //modify if needed
        explicit IRGenerator(const std::string moduleName) : ASTVisitor(), TheModule(), TheBuilder(), ModuleName(moduleName){
            TheContext = std::make_unique<llvm::LLVMContext>();
        }

        
        std::unique_ptr<llvm::Module> getModule() {
            TheBuilder.reset();
            return std::move(TheModule);
        }

        void visitProgram(Program* prog) override;

        void visitVarDecl(VarDeclaration *decl) override;

        void visitFuncDecl(FuncDeclaration *func) override;

        void visitIfStmt(IfStatement *stmt) override;

        void visitForStmt(ForStatement *stmt) override;

        void visitWhileStmt(WhileStatement *stmt) override;

        void visitReturnStmt(ReturnStatement *stmt) override;

        void visitBreakStmt(BreakStatement *stmt) override;

        void visitScope(ScopeStatement *stmt) override;

        void visitUnaryExpr(UnaryExpr *expr) override;

        void visitBinaryExpr(BinaryExpr *expr) override;

        void visitCallExpr(CallExpr *expr) override;

        void visitVarExpr(VarExpr *expr) override;

        void visitAssignmentExpr(AssignmentExpr *expr) override;

        void visitIntLiteralExpr(IntLiteralExpr *literal) override;

        void visitBoolLiteralExpr(BoolLiteralExpr *literal) override;

        void visitCharLiteralExpr(CharLiteralExpr *literal) override;
    };
}

#endif //MINICC_IRGENERATOR_H
