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
    llvm::Type *IRGenerator::miniccTypeTollvmTypePrimitive(minicc::Type type)
    {
        if (type == Type(Type::Bool))
            return llvm::Type::getInt1Ty(*TheContext);
        if (type == Type(Type::Char))
            return llvm::Type::getInt8Ty(*TheContext);
        if (type == Type(Type::Int))
            return llvm::Type::getInt32Ty(*TheContext);
        if (type == Type(Type::Void))
            return llvm::Type::getVoidTy(*TheContext);
        llvm::errs() << "Unknown type: " << type.toString() << "\n"; // Add this
        return nullptr;                                              // Explicit return
    }

    llvm::Type *IRGenerator::miniccTypeTollvmType(minicc::Type type)
    {
        llvm::Type *llvmType = miniccTypeTollvmTypePrimitive(Type(type.primitiveType()));
        if (type.arrayBound() == 0)
        {
            return llvmType;
        }
        return llvm::ArrayType::get(llvmType, type.arrayBound());
    }

    // Code from LLVM tutorial, used to allocate on the stack
    llvm::AllocaInst *IRGenerator::CreateEntryBlockAlloca(llvm::Function *function, std::string name,
                                                          llvm::Type *type)
    {
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

        if (prog->syslibFlag())
        {
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
        for (int i = 0; i < prog->numChildren(); i++)
        {
            auto child = prog->getChild(i);
            child->accept(this);
        }
    }

    void IRGenerator::visitVarDecl(VarDeclaration *decl)
    {
        // start your code here
        // How to know if a declaration is local or global?
        // For each of the variables (since there can be multiple declared),
        // need to create global variable for them
        // variable = llvm::GlobalVariable(*TheModule, miniccTypeTollvmType(decl->declType()),
        // false, llvm::GlobalVariable::CommonLinkage, nullptr, decl->
        auto parent = decl->getParent();
        auto table = decl->getParentScope()->scopeVarTable();
        auto parentBB = TheBuilder->GetInsertBlock()->getParent();
        for (size_t i = 0; i < decl->numVarReferences(); i++)
        {
            VarReference *ref = (VarReference *)decl->varReference(i);

            auto name = ref->identifier()->name();
            auto type = miniccTypeTollvmType(Type(decl->declType(), ref->isArray()));

            VarSymbolEntry *entry = table->lookup(name);
            // TODO: support array
            if (parent->isProgram())
                entry->LLVMValue = new llvm::GlobalVariable(*TheModule, type, false,
                                                            llvm::GlobalVariable::CommonLinkage, nullptr, name);
            else
                entry->LLVMValue = CreateEntryBlockAlloca(parentBB, name, type);
        }
    }

    void IRGenerator::visitFuncDecl(FuncDeclaration *func)
    {
        // start your code here
        // Since semantic analysis has already been performed, we probably don't
        // need to do any error checking here

        // • Get the corresponding llvm::Function object.
        llvm::Function *function = TheModule->getFunction(func->name());
        // • Check the function declaration has body or not. If so, allocate
        if (!function)
        {
            llvm::Type *returnType = miniccTypeTollvmType(func->returnType());
            std::vector<llvm::Type *> argTypes;
            for (int i = 0; i < func->numParameters(); i++)
            {
                argTypes.emplace_back(miniccTypeTollvmType(func->parameter(i)->type()));
            }

            llvm::FunctionType *funcType = llvm::FunctionType::get(returnType, argTypes, false);
            function = llvm::Function::Create(
                funcType, llvm::Function::ExternalLinkage, func->name(), TheModule.get());
        }

        // Return if no body
        if (!func->hasBody())
        {
            return;
        }

        llvm::FunctionType *funcType = function->getFunctionType();

        llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "entry", function);
        TheBuilder->SetInsertPoint(BB);

        auto table = func->body()->scopeVarTable();

        // allocate params
        for (int i = 0; i < func->numParameters(); i++)
        {
            auto param = func->parameter(i);
            llvm::Value *alloc = CreateEntryBlockAlloca(function, param->name(),
                                                        funcType->getParamType(i));
            if (alloc)
            {
                llvm::Argument *arg = function->arg_begin() + i;
                arg->setName(param->name());
                TheBuilder->CreateStore(arg, alloc);
                // symbolTable[param->name()] = alloc; // Store in a symbol table (e.g., std::map<std::string, llvm::Value*>)
            }

            table->lookup(param->name())->LLVMValue = alloc;
        }

        // • If having body, a entry basic block should be created for the function
        // and inserted in TheBuilder.

        // Should traverse the body now
        func->body()->accept(this);

        // • If having body but no return expr in void function, create a void
        // return for it. Check if BB has a return
        auto currentBB = TheBuilder->GetInsertBlock();
        // This is getting inserted to the wrong block
        if (!currentBB->getTerminator())
        {
            TheBuilder->CreateRetVoid();
        }

        llvm::verifyFunction(*function, &llvm::errs());
    }

    void IRGenerator::visitIfStmt(IfStatement *stmt)
    {
        // start your code here
        llvm::Function *function = TheBuilder->GetInsertBlock()->getParent();
        // llvm::BasicBlock *slowBB = llvm::BasicBlock::Create(*TheContext, "slow", function);
        auto thenBB = llvm::BasicBlock::Create(*TheContext, "then", function);
        llvm::BasicBlock *elseBB;
        if (stmt->hasElse()) {
            elseBB = llvm::BasicBlock::Create(*TheContext, "else", function);
        }
        // Should this be inserted before the inner blocks are created?
        auto outBB = llvm::BasicBlock::Create(*TheContext, "out", function);
        
        stmt->condExpr()->accept(this);
        auto condVal = LLVMValueForExpr[stmt->condExpr()];
        TheBuilder->CreateCondBr(condVal, thenBB, stmt->hasElse() ? elseBB : outBB);
        
        TheBuilder->SetInsertPoint(thenBB);
        stmt->thenStmt()->accept(this);
        TheBuilder->CreateBr(outBB);

        if (stmt->hasElse()) {
            TheBuilder->SetInsertPoint(elseBB);
            stmt->elseStmt()->accept(this);
            TheBuilder->CreateBr(outBB);
        }
        TheBuilder->SetInsertPoint(outBB);
    }

    void IRGenerator::visitForStmt(ForStatement *stmt)
    {
        // start your code here
        auto function = TheBuilder->GetInsertBlock()->getParent();
        auto condBB = llvm::BasicBlock::Create(*TheContext, "cond", function);
        auto bodyBB = llvm::BasicBlock::Create(*TheContext, "body", function);
        auto outBB = llvm::BasicBlock::Create(*TheContext, "out", function);
        stmt->initExpr()->accept(this);
        
        TheBuilder->CreateBr(condBB);
        TheBuilder->SetInsertPoint(condBB);
        stmt->condExpr()->accept(this);
        assert(LLVMValueForExpr[stmt->condExpr()]);
        TheBuilder->CreateCondBr(LLVMValueForExpr[stmt->condExpr()], bodyBB, outBB);
        
        TheBuilder->SetInsertPoint(bodyBB);
        stmt->body()->accept(this);
        stmt->iterExpr()->accept(this);
        TheBuilder->CreateBr(condBB);
        
        TheBuilder->SetInsertPoint(outBB);
    }
    
    void IRGenerator::visitWhileStmt(WhileStatement *stmt)
    {
        // start your code here
        auto function = TheBuilder->GetInsertBlock()->getParent();
        auto condBB = llvm::BasicBlock::Create(*TheContext, "cond", function);
        auto bodyBB = llvm::BasicBlock::Create(*TheContext, "body", function);
        auto outBB = llvm::BasicBlock::Create(*TheContext, "out", function);
        TheBuilder->CreateBr(condBB);
        TheBuilder->SetInsertPoint(condBB);
        stmt->condExpr()->accept(this);
        assert(LLVMValueForExpr[stmt->condExpr()]);
        TheBuilder->CreateCondBr(LLVMValueForExpr[stmt->condExpr()], bodyBB, outBB);
        TheBuilder->SetInsertPoint(bodyBB);
        stmt->body()->accept(this);
        TheBuilder->CreateBr(condBB);
        
        TheBuilder->SetInsertPoint(outBB);
    }

    void IRGenerator::visitReturnStmt(ReturnStatement *stmt)
    {
        // start your code here
        if (!stmt->hasReturnExpr()) {
            TheBuilder->CreateRetVoid();
            return;
        }
        Expr *expr = stmt->returnExpr();
        expr->accept(this);
        assert(LLVMValueForExpr[expr]);
        TheBuilder->CreateRet(LLVMValueForExpr[expr]);
    }

    void IRGenerator::visitBreakStmt(BreakStatement *stmt)
    {

        // start your code here
    }

    void IRGenerator::visitUnaryExpr(UnaryExpr *expr)
    {
        // start your code here
        Expr *subexpr = (Expr *)expr->getChild(0);
        subexpr->accept(this);
        llvm::Value *value = LLVMValueForExpr[subexpr]; // this Value may not have int type
        assert(value);
        if (expr->opcode() == Expr::ExprOpcode::Sub)
        {
            LLVMValueForExpr[expr] = TheBuilder->CreateNeg(value);
        }
        if (expr->opcode() == Expr::ExprOpcode::Not)
        {
            LLVMValueForExpr[expr] = TheBuilder->CreateNot(value);
        }
    }

    void IRGenerator::visitBinaryExpr(BinaryExpr *expr)
    {
        // start your code here
        if (expr->opcode() != Expr::ExprOpcode::And && expr->opcode() != Expr::ExprOpcode::Or)
        {
            expr->getChild(0)->accept(this);
            expr->getChild(1)->accept(this);
            auto leftVal = LLVMValueForExpr[(Expr *)expr->getChild(0)];
            auto rightVal = LLVMValueForExpr[(Expr *)expr->getChild(1)];
            llvm::Value *resVal;
            switch (expr->opcode())
            {
            case Expr::ExprOpcode::Add:
                resVal = TheBuilder->CreateAdd(leftVal, rightVal);
                break;
            case Expr::ExprOpcode::Sub:
                resVal = TheBuilder->CreateSub(leftVal, rightVal);
                break;
            case Expr::ExprOpcode::Mul:
                resVal = TheBuilder->CreateMul(leftVal, rightVal);
                break;
            case Expr::ExprOpcode::Div:
                resVal = TheBuilder->CreateSDiv(leftVal, rightVal);
                break;
            case Expr::ExprOpcode::Equal:
                resVal = TheBuilder->CreateICmpEQ(leftVal, rightVal);
                break;
            case Expr::ExprOpcode::NotEqual:
                resVal = TheBuilder->CreateICmpNE(leftVal, rightVal);
                break;
            case Expr::ExprOpcode::Less:
                resVal = TheBuilder->CreateICmpSLT(leftVal, rightVal);
                break;
            case Expr::ExprOpcode::LessEqual:
                resVal = TheBuilder->CreateICmpSLE(leftVal, rightVal);
                break;
            case Expr::ExprOpcode::Greater:
                resVal = TheBuilder->CreateICmpSGT(leftVal, rightVal);
                break;
            case Expr::ExprOpcode::GreaterEqual:
                resVal = TheBuilder->CreateICmpSGE(leftVal, rightVal);
                break;
            }
            LLVMValueForExpr[expr] = resVal;
            return;
        }

        // Handle "and" and "or"
        auto function = TheBuilder->GetInsertBlock()->getParent();
        auto leftChild = (Expr *)expr->getChild(0);
        leftChild->accept(this);
        auto leftVal = LLVMValueForExpr[leftChild];
        assert(leftVal);

        // Todo: why does it have to be a function? What if nested if statements?
        llvm::BasicBlock *slowBB = llvm::BasicBlock::Create(*TheContext, "slow", function);
        llvm::BasicBlock *outBB = llvm::BasicBlock::Create(*TheContext, "out", function);
        // Do comparison here
        auto trueVal = llvm::ConstantInt::get(llvm::Type::getInt1Ty(*TheContext), 1);
        auto falseVal = llvm::ConstantInt::get(llvm::Type::getInt1Ty(*TheContext), 0);

        auto currentBB = TheBuilder->GetInsertBlock();

        auto comp = TheBuilder->CreateICmpEQ(leftVal, trueVal);
        
        bool isAnd = (expr->opcode() == Expr::ExprOpcode::And);

        llvm::BasicBlock *trueBB = isAnd ? slowBB : outBB;  // And: true -> slow, Or: true -> out
        llvm::BasicBlock *falseBB = isAnd ? outBB : slowBB; //

        TheBuilder->CreateCondBr(comp, trueBB, falseBB);
        
        // slow block
        TheBuilder->SetInsertPoint(slowBB);
        auto rightChild = (Expr *)expr->getChild(1);
        rightChild->accept(this);
        auto rightVal = LLVMValueForExpr[rightChild];
        auto rightComp = TheBuilder->CreateICmpEQ(rightVal, trueVal);
        TheBuilder->CreateBr(outBB);
        
        // out block
        // function->insert(function->end(), outBB);
        TheBuilder->SetInsertPoint(outBB);
        llvm::PHINode *phiNode = TheBuilder->CreatePHI(llvm::Type::getInt1Ty(*TheContext), 2);
        
        phiNode->addIncoming(rightComp, slowBB);
        phiNode->addIncoming(isAnd ? falseVal : trueVal, currentBB);

        LLVMValueForExpr[expr] = phiNode;
    }

    void IRGenerator::visitCallExpr(CallExpr *expr)
    {
        // start your code here
        llvm::Function *func = TheModule->getFunction(expr->callee()->name());
        assert(func);
        std::vector<llvm::Value *> argsV;
        for (int i = 0; i < expr->numArgs(); i++)
        {
            expr->arg(i)->accept(this);
            assert(LLVMValueForExpr[expr->arg(i)]);
            argsV.emplace_back(LLVMValueForExpr[expr->arg(i)]);
        }
        TheBuilder->CreateCall(func, argsV);
    }

    void IRGenerator::visitVarExpr(VarExpr *expr)
    {
        // start your code here
        // Look up symbol table?
        auto ref = (VarReference *)expr->getChild(0);
        auto table = expr->locateDeclaringTableForVar(ref->identifier()->name());
        llvm::Value *value = table->lookup(ref->identifier()->name())->LLVMValue;

        // Are we supposed to be doing this?
        assert(value);

        // TODO: need to use CreateGEP for array
        // if (expr->exprType().arrayBound > 0) {
        //     TheBuilder::CreateGEP()
        // }
        LLVMValueForExpr[expr] = TheBuilder->CreateLoad(miniccTypeTollvmType(expr->exprType()),
                                                        value);
    }

    void IRGenerator::visitAssignmentExpr(AssignmentExpr *expr)
    {
        // • Get variable llvm::Value
        // • CreateGEP if it is array.
        // • CreateStore to assign the right value to the variable.
        // start your code here
        expr->getChild(1)->accept(this);
        VarReference *reference = (VarReference *)expr->getChild(0);
        VarSymbolTable *table = expr->locateDeclaringTableForVar(reference->identifier()->name());
        assert(table);
        VarSymbolEntry *entry = table->lookup(reference->identifier()->name());
        assert(entry);
        assert(entry->LLVMValue);
        assert(LLVMValueForExpr[(Expr *)expr->getChild(1)]);
        TheBuilder->CreateStore(LLVMValueForExpr[(Expr *)expr->getChild(1)], entry->LLVMValue);
        LLVMValueForExpr[expr] = entry->LLVMValue;
    }

    void IRGenerator::visitIntLiteralExpr(IntLiteralExpr *literal)
    {
        // start your code here
        llvm::Constant *constant = llvm::ConstantInt::get(
            llvm::Type::getInt32Ty(*TheContext), literal->value(), false);
        LLVMValueForExpr.emplace(literal, constant);
    }

    void IRGenerator::visitBoolLiteralExpr(BoolLiteralExpr *literal)
    {
        // start your code here
        llvm::Constant *constant = llvm::ConstantInt::get(
            llvm::Type::getInt1Ty(*TheContext), literal->value(), false);
        LLVMValueForExpr.emplace(literal, constant);
    }

    void IRGenerator::visitCharLiteralExpr(CharLiteralExpr *literal)
    {
        // start your code here
        llvm::Constant *constant = llvm::ConstantInt::get(
            llvm::Type::getInt8Ty(*TheContext), literal->value(), false);
        LLVMValueForExpr.emplace(literal, constant);
    }

    void IRGenerator::visitScope(ScopeStatement *stmt)
    {
        // start your code here
        // Before visiting child nodes, note that there may be a "return" statement in the middle of a scope.
        // Do we need to do anything if there is a return in the middle of a scope?
        // Just stop?
        llvm::BasicBlock *currentBB = TheBuilder->GetInsertBlock();
        for (int i = 0; i < stmt->numChildren(); i++)
        {
            auto child = stmt->getChild(i);
            child->accept(this);
            // TODO: this is causing early termination, should the block have a terminator?
            // if (currentBB->getTerminator())
            // {
            //     std::cout << "Encountered return at child " + std::to_string(i) << std::endl;
            //     break;
            // }
        }
    }

}