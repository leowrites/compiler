//
// Created by Fan Long on 2020/12/6.
//

// add more header files if your want
#include "IRGenerator.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "Declarations.h"
#include "Program.h"
#include "Exprs.h"
#include "Statements.h"

namespace minicc
{
    // add your member helper functions
    llvm::Type* IRGenerator::miniccTypeTollvmTypePrimitive(minicc::Type type) {
        if (type == Type(Type::Bool))
        return llvm::Type::getInt1Ty(*TheContext);
        if (type == Type(Type::Char))
        return llvm::Type::getInt8Ty(*TheContext);
        if (type == Type(Type::Int))
        return llvm::Type::getInt32Ty(*TheContext);
        if (type == Type(Type::Void))
        return llvm::Type::getVoidTy(*TheContext);
    }

    llvm::Type* IRGenerator::miniccTypeTollvmType(minicc::Type type) {
        llvm::Type *llvmType = miniccTypeTollvmTypePrimitive(Type(type.primitiveType()));
        if (type.arrayBound() == 0) {
            return llvmType;
        }
        return llvm::ArrayType::get(llvmType, type.arrayBound());
    }
    
    // Code from LLVM tutorial, used to allocate on the stack
    llvm::AllocaInst * IRGenerator::CreateEntryBlockAlloca(llvm::Function *function, std::string name, 
        llvm::Type *type) {
        assert(function);
        assert(type);
        llvm::IRBuilder<> TmpB(&function->getEntryBlock(), function->getEntryBlock().begin());
        return TmpB.CreateAlloca(type, nullptr, name);
    }

    void IRGenerator::visitProgram(Program *prog)
    {
        // Initlize llvm module and builder
        TheModule = std::make_unique<llvm::Module>(ModuleName, *TheContext);
        TheBuilder = std::make_unique<llvm::IRBuilder<>>(*TheContext);
        // start your code here

        if (prog->syslibFlag()) {
            llvm::FunctionType *getIntType = llvm::FunctionType::get(
                llvm::Type::getInt32Ty(*TheContext), std::vector<llvm::Type *>(), false);
            llvm::Function *getIntFunc = llvm::Function::Create(
                getIntType, llvm::Function::ExternalLinkage, "getint", TheModule.get());
            
            std::vector<llvm::Type *> Int(1, llvm::Type::getInt32Ty(*TheContext));
            llvm::FunctionType *putIntType = llvm::FunctionType::get(
                llvm::Type::getVoidTy(*TheContext), Int, false);
            llvm::Function *putIntFunc = llvm::Function::Create(
                putIntType, llvm::Function::ExternalLinkage, "putint", TheModule.get());
    
            // TODO: is this type correct? Should it be 8
            std::vector<llvm::Type *> Char(1, llvm::Type::getInt8Ty(*TheContext));
            llvm::FunctionType *putcharacterType = llvm::FunctionType::get(
                llvm::Type::getVoidTy(*TheContext), Char, false);
            llvm::Function *putcharacterFunc = llvm::Function::Create(
                putcharacterType, llvm::Function::ExternalLinkage, "putcharacter", TheModule.get());
    
            llvm::FunctionType *putnewlineType = llvm::FunctionType::get(
                llvm::Type::getVoidTy(*TheContext), std::vector<llvm::Type *>(), false);
            llvm::Function *putnewlineFunc = llvm::Function::Create(
                putnewlineType, llvm::Function::ExternalLinkage, "putnewline", TheModule.get());
        }
        // Why is there no visitASTNode?
        for (int i = 0; i < prog->numChildren(); i++) {
            auto child = prog->getChild(i);
            child->accept(this);
        }
    }

    void IRGenerator::visitVarDecl(VarDeclaration *decl)
    {
        // start your code here
    }

    void IRGenerator::visitFuncDecl(FuncDeclaration *func)
    {
        // start your code here
        // Since semantic analysis has already been performed, we probably don't
        // need to do any error checking here
        
        // • Get the corresponding llvm::Function object.
        llvm::Function *function = TheModule->getFunction(func->name());
        // • Check the function declaration has body or not. If so, allocate
        if (!function) {
            llvm::Type *returnType = miniccTypeTollvmType(func->returnType());
            std::vector<llvm::Type *> argTypes;
            for (int i = 0; i < func->numParameters(); i++) {
                argTypes.emplace_back(miniccTypeTollvmType(func->parameter(i)->type()));
            }
            
            llvm::FunctionType *funcType = llvm::FunctionType::get(returnType, argTypes, false);
            function = llvm::Function::Create(
                funcType, llvm::Function::ExternalLinkage, func->name(), TheModule.get()
            );
        }
        
        // Return if no body
        if (!func->hasBody()) {
            return;
        }
        
        llvm::FunctionType *funcType = function->getFunctionType();
        

        llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "entry", function);
        TheBuilder->SetInsertPoint(BB);

        // allocate params
        for (int i = 0; i < func->numParameters(); i++) {
            // TODO: may need to store this somewhere
            auto param = func->parameter(i);
            llvm::Value *alloc = CreateEntryBlockAlloca(function, param->name(), 
                funcType->getParamType(i));
            if (alloc) {
                llvm::Argument *arg = function->arg_begin() + i;
                arg->setName(param->name());
                TheBuilder->CreateStore(arg, alloc);
                // symbolTable[param->name()] = alloc; // Store in a symbol table (e.g., std::map<std::string, llvm::Value*>)
            }
        }
        
        // • If having body, a entry basic block should be created for the function
        // and inserted in TheBuilder.

        // Should traverse the body now
        func->body()->accept(this);
        
        // • If having body but no return expr in void function, create a void
        // return for it. Check if BB has a return
        if (!BB->getTerminator()) {
            TheBuilder->CreateRetVoid();
        }

        llvm::verifyFunction(*function, &llvm::errs());
    }

    void IRGenerator::visitIfStmt(IfStatement *stmt)
    {
        // start your code here
    }

    void IRGenerator::visitForStmt(ForStatement *stmt)
    {
        // start your code here
    }

    void IRGenerator::visitWhileStmt(WhileStatement *stmt)
    {
        // start your code here
    }

    void IRGenerator::visitReturnStmt(ReturnStatement *stmt)
    {
        // start your code here
    }

    void IRGenerator::visitBreakStmt(BreakStatement *stmt)
    {
        // start your code here
    }

    void IRGenerator::visitUnaryExpr(UnaryExpr *expr)
    {
        // start your code here
    }

    void IRGenerator::visitBinaryExpr(BinaryExpr *expr)
    {
        // start your code here
    }

    void IRGenerator::visitCallExpr(CallExpr *expr)
    {
        // start your code here
    }

    void IRGenerator::visitVarExpr(VarExpr *expr)
    {
        // start your code here
    }

    void IRGenerator::visitAssignmentExpr(AssignmentExpr *expr)
    {
        // start your code here
    }
    
    void IRGenerator::visitIntLiteralExpr(IntLiteralExpr *literal)
    {
        // start your code here
        llvm::Constant* constant = llvm::ConstantInt::get(
            llvm::Type::getInt32Ty(*TheContext), literal->value(), false
        );
        LLVMValueForExpr.emplace(literal, constant);
    }

    void IRGenerator::visitBoolLiteralExpr(BoolLiteralExpr *literal)
    {
        // start your code here
        llvm::Constant* constant = llvm::ConstantInt::get(
            llvm::Type::getInt1Ty(*TheContext), literal->value(), false
        );
        LLVMValueForExpr.emplace(literal, constant);
    }

    void IRGenerator::visitCharLiteralExpr(CharLiteralExpr *literal)
    {
        // start your code here
        llvm::Constant* constant = llvm::ConstantInt::get(
            llvm::Type::getInt8Ty(*TheContext), literal->value(), false
        );
        LLVMValueForExpr.emplace(literal, constant);
    }

    void IRGenerator::visitScope(ScopeStatement *stmt)
    {
        // start your code here
    }

}