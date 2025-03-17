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
#include "iostream"

namespace minicc
{
    // add your member helper functions

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
            llvm::Function::Create(getIntType, llvm::Function::ExternalLinkage, "getint", TheModule.get());

            std::vector<llvm::Type *> Int(1, llvm::Type::getInt32Ty(*TheContext));
            llvm::FunctionType *putIntType = llvm::FunctionType::get(
                llvm::Type::getVoidTy(*TheContext), Int, false);
            llvm::Function::Create(putIntType, llvm::Function::ExternalLinkage, "putint", TheModule.get());

            std::vector<llvm::Type *> Char(1, llvm::Type::getInt8Ty(*TheContext));
            llvm::FunctionType *putcharacterType = llvm::FunctionType::get(
                llvm::Type::getVoidTy(*TheContext), Char, false);
            llvm::Function::Create(putcharacterType, llvm::Function::ExternalLinkage, "putcharacter", TheModule.get());

            llvm::FunctionType *putnewlineType = llvm::FunctionType::get(
                llvm::Type::getVoidTy(*TheContext), std::vector<llvm::Type *>(), false);
            llvm::Function::Create(putnewlineType, llvm::Function::ExternalLinkage, "putnewline", TheModule.get());
        }

        for (int i = 0; i < prog->numChildren(); i++)
            prog->getChild(i)->accept(this);
    }

    void IRGenerator::visitVarDecl(VarDeclaration *decl)
    {
        // start your code here
        auto table = decl->getParentScope()->scopeVarTable();
        assert(table);
        auto parentBB = getFunctionBlock();

        for (size_t i = 0; i < decl->numVarReferences(); i++)
        {
            VarReference *ref = decl->varReference(i);
            auto name = ref->identifier()->name();
            VarSymbolEntry *entry = table->lookup(name);
            assert(entry);

            auto type = miniccTypeTollvmType(Type(decl->declType(), entry->VarType.arrayBound()));

            if (!parentBB) {
                llvm::Constant* initializer = llvm::Constant::getNullValue(type);
                entry->LLVMValue = new llvm::GlobalVariable(*TheModule, type, false,
                                            llvm::GlobalVariable::CommonLinkage, initializer, name);
            }
            else {
                llvm::Value *arraySize = nullptr;
                if (ref->isArray()) {
                    ref->indexExpr()->accept(this);
                    arraySize = LLVMValueForExpr[ref->indexExpr()];
                }
                entry->LLVMValue = CreateEntryBlockAlloca(parentBB, name, type, arraySize);
            }
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
            for (int i = 0; i < func->numParameters(); i++) {
                argTypes.emplace_back(miniccTypeTollvmType(func->parameter(i)->type()));
            }

            llvm::FunctionType *funcType = llvm::FunctionType::get(returnType, argTypes, false);
            function = llvm::Function::Create(
                funcType, llvm::Function::ExternalLinkage, func->name(), TheModule.get());
        }

        if (!func->hasBody())
            return;

        llvm::FunctionType *funcType = function->getFunctionType();

        llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "entry", function);
        TheBuilder->SetInsertPoint(BB);

        auto table = func->body()->scopeVarTable();

        // allocate params
        for (int i = 0; i < func->numParameters(); i++)
        {
            auto param = func->parameter(i);
            llvm::Value *alloc = CreateEntryBlockAlloca(function, param->name(),
                                                        funcType->getParamType(i), nullptr);
            assert(alloc);
            llvm::Argument *arg = function->arg_begin() + i;
            arg->setName(param->name());
            TheBuilder->CreateStore(arg, alloc);

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
        auto thenBB = llvm::BasicBlock::Create(*TheContext, "then", function);
        llvm::BasicBlock *elseBB;
        if (stmt->hasElse()) {
            elseBB = llvm::BasicBlock::Create(*TheContext, "else", function);
        }
        auto outBB = llvm::BasicBlock::Create(*TheContext, "out", function);

        stmt->condExpr()->accept(this);
        auto condVal = LLVMValueForExpr[stmt->condExpr()];
        TheBuilder->CreateCondBr(condVal, thenBB, stmt->hasElse() ? elseBB : outBB);

        TheBuilder->SetInsertPoint(thenBB);
        stmt->thenStmt()->accept(this);
        checkTerminatorAndCreateBr(outBB);

        if (stmt->hasElse()) {
            TheBuilder->SetInsertPoint(elseBB);
            stmt->elseStmt()->accept(this);
            checkTerminatorAndCreateBr(outBB);
        }
        TheBuilder->SetInsertPoint(outBB);
    }

    void IRGenerator::visitForStmt(ForStatement *stmt)
    {
        // start your code here
        auto function = TheBuilder->GetInsertBlock()->getParent();
        auto bodyBB = llvm::BasicBlock::Create(*TheContext, "body", function);
        auto outBB = llvm::BasicBlock::Create(*TheContext, "out", function);
        auto hasCond = stmt->condExpr() != nullptr;

        LoopExitStack.emplace_back(outBB);

        if (stmt->initExpr())
            stmt->initExpr()->accept(this);

        llvm::BasicBlock *condBB;
        if (hasCond) {
            condBB = llvm::BasicBlock::Create(*TheContext, "cond", function);
            TheBuilder->CreateBr(condBB);
            TheBuilder->SetInsertPoint(condBB);
            stmt->condExpr()->accept(this);
            assert(LLVMValueForExpr[stmt->condExpr()]);
            TheBuilder->CreateCondBr(LLVMValueForExpr[stmt->condExpr()], bodyBB, outBB);
        } else {
            TheBuilder->CreateBr(bodyBB);
        }

        TheBuilder->SetInsertPoint(bodyBB);
        stmt->body()->accept(this);

        if (!TheBuilder->GetInsertBlock()->getTerminator()) {
            if (stmt->iterExpr())
                stmt->iterExpr()->accept(this);
            if (hasCond)
                checkTerminatorAndCreateBr(condBB);
            else
                checkTerminatorAndCreateBr(bodyBB);
        }

        TheBuilder->SetInsertPoint(outBB);
        LoopExitStack.pop_back();
    }

    void IRGenerator::visitWhileStmt(WhileStatement *stmt)
    {
        // start your code here
        auto function = TheBuilder->GetInsertBlock()->getParent();
        auto condBB = llvm::BasicBlock::Create(*TheContext, "cond", function);
        auto bodyBB = llvm::BasicBlock::Create(*TheContext, "body", function);
        auto outBB = llvm::BasicBlock::Create(*TheContext, "out", function);
        LoopExitStack.emplace_back(outBB);

        TheBuilder->CreateBr(condBB);
        TheBuilder->SetInsertPoint(condBB);
        stmt->condExpr()->accept(this);
        assert(LLVMValueForExpr[stmt->condExpr()]);
        TheBuilder->CreateCondBr(LLVMValueForExpr[stmt->condExpr()], bodyBB, outBB);

        TheBuilder->SetInsertPoint(bodyBB);
        stmt->body()->accept(this);
        checkTerminatorAndCreateBr(condBB);

        TheBuilder->SetInsertPoint(outBB);
        LoopExitStack.pop_back();
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
        assert(LoopExitStack.back());
        TheBuilder->CreateBr(LoopExitStack.back());
    }

    void IRGenerator::visitUnaryExpr(UnaryExpr *expr)
    {
        // start your code here
        Expr *subexpr = (Expr *)expr->getChild(0);
        subexpr->accept(this);
        llvm::Value *value = LLVMValueForExpr[subexpr];
        assert(value);
        if (expr->opcode() == Expr::ExprOpcode::Sub)
            LLVMValueForExpr[expr] = TheBuilder->CreateNeg(value);
        if (expr->opcode() == Expr::ExprOpcode::Not)
            LLVMValueForExpr[expr] = TheBuilder->CreateNot(value);
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
        LLVMValueForExpr[expr] = TheBuilder->CreateCall(func, argsV);
    }

    void IRGenerator::visitVarExpr(VarExpr *expr)
    {
        // start your code here
        // Look up symbol table?
        auto ref = (VarReference *)expr->getChild(0);
        assert(ref);

        auto name = ref->identifier()->name();
        auto table = expr->locateDeclaringTableForVar(name);
        auto entry = table->lookup(name);
        llvm::Value *value = entry->LLVMValue;

        assert(value);
        llvm::Type *type = miniccTypeTollvmType(entry->VarType);
        if (ref->isArray()) {
            ref->indexExpr()->accept(this);
            auto zero = llvm::ConstantInt::get(llvm::Type::getInt8Ty(*TheContext), 0, false);
            std::vector<llvm::Value *> indexList = { zero ,LLVMValueForExpr[ref->indexExpr()] };
            llvm::Value *arrayPointer = TheBuilder->CreateGEP(type, value, indexList);
            LLVMValueForExpr[expr] = TheBuilder->CreateLoad(miniccTypeTollvmType(expr->exprType()), arrayPointer);
        } else {
            LLVMValueForExpr[expr] = TheBuilder->CreateLoad(type, value);
        }
    }

    void IRGenerator::visitAssignmentExpr(AssignmentExpr *expr)
    {
        // • Get variable llvm::Value
        // • CreateGEP if it is array.
        // • CreateStore to assign the right value to the variable.
        // start your code here
        auto ref = (VarReference *)expr->getChild(0);
        expr->getChild(1)->accept(this); // visit expression
        auto name = ref->identifier()->name();
        VarSymbolTable *table = expr->locateDeclaringTableForVar(name);
        VarSymbolEntry *entry = table->lookup(name);

        llvm::Value *value = entry->LLVMValue;
        if (ref->isArray()) {
            ref->indexExpr()->accept(this);
            auto zero = llvm::ConstantInt::get(llvm::Type::getInt8Ty(*TheContext), 0, false);
            std::vector<llvm::Value *> indexList = { zero, LLVMValueForExpr[ref->indexExpr()] };
            value = TheBuilder->CreateGEP(miniccTypeTollvmType(entry->VarType), value, indexList);
        }
        assert(entry);
        assert(entry->LLVMValue);
        assert(LLVMValueForExpr[(Expr *)expr->getChild(1)]);
        TheBuilder->CreateStore(LLVMValueForExpr[(Expr *)expr->getChild(1)], value);
        LLVMValueForExpr[expr] = LLVMValueForExpr[(Expr *)expr->getChild(1)];
    }

    void IRGenerator::visitIntLiteralExpr(IntLiteralExpr *literal)
    {
        // start your code here
        llvm::Constant *constant = llvm::ConstantInt::get(
            llvm::Type::getInt32Ty(*TheContext), literal->value(), false);
        LLVMValueForExpr[literal] = constant;
    }

    void IRGenerator::visitBoolLiteralExpr(BoolLiteralExpr *literal)
    {
        // start your code here
        llvm::Constant *constant = llvm::ConstantInt::get(
            llvm::Type::getInt1Ty(*TheContext), literal->value(), false);
        LLVMValueForExpr[literal] = constant;
    }

    void IRGenerator::visitCharLiteralExpr(CharLiteralExpr *literal)
    {
        // start your code here
        llvm::Constant *constant = llvm::ConstantInt::get(
            llvm::Type::getInt8Ty(*TheContext), literal->value(), false);
        LLVMValueForExpr[literal] = constant;
    }

    void IRGenerator::visitScope(ScopeStatement *stmt)
    {
        // start your code here
        // Before visiting child nodes, note that there may be a "return" statement in the middle of a scope.
        // Do we need to do anything if there is a return in the middle of a scope?
        // Just stop?
        llvm::BasicBlock *currentBB;
        for (int i = 0; i < stmt->numChildren(); i++)
        {
            auto child = stmt->getChild(i);
            child->accept(this);
            // If there is a terminator here, we should not continue
            currentBB = TheBuilder->GetInsertBlock();
            if (currentBB->getTerminator())
                break;
        }
    }

}