//
// Created by Fan Long on 2020/12/4.
//

// Add necessary headers you want
#include "VerifyAndBuildSymbols.h"
#include "Declarations.h"
#include "Terms.h"
#include "Types.h"
#include "Exprs.h"
#include "Statements.h"
#include "Program.h"
#include <string>
#include <sstream>
#include <set>
#include <iostream>

namespace minicc
{

    void VerifyAndBuildSymbols::visitASTNode(ASTNode *node)
    {
        // start your code here
        // Hint: set root of the node before visiting children
        for (size_t i = 0; i < node->numChildren(); i++)
        {
            auto child = node->getChild(i);
            if (child)
            {
                if (node->isProgram())
                {
                    child->setRoot((Program *)node);
                }
                else
                {
                    child->setRoot(node->root());
                }
                child->setParent(node);
                child->accept(this);
            }
        }
    }

    void VerifyAndBuildSymbols::visitProgram(Program *prog)
    {
        // start your code here
        if (prog->syslibFlag())
        {
            FuncSymbolTable *funcTable = prog->funcTable();
            FuncSymbolEntry getintEntry(Type(Type::Int), std::vector<Type>(), false);
            funcTable->insert("getint", getintEntry);

            std::vector<Type> putintParams;
            putintParams.push_back(Type(Type::Int));
            FuncSymbolEntry putintEntry(Type(Type::Void), putintParams, false);
            funcTable->insert("putint", putintEntry);

            std::vector<Type> putcharParams;
            putcharParams.push_back(Type(Type::Char));
            FuncSymbolEntry putcharacterEntry(Type(Type::Void), putcharParams, false);
            funcTable->insert("putcharacter", putcharacterEntry);

            FuncSymbolEntry putnewlineEntry(Type(Type::Void), std::vector<Type>(), false);
            funcTable->insert("putnewline", putnewlineEntry);
        }
        this->visitASTNode(prog);
    }

    void VerifyAndBuildSymbols::visitVarDecl(VarDeclaration *decl)
    {
        // start your code here
        // Hint: Check that same variable cannot be declared twice in the same scope
        //  create an entry for each reference in the var table

        this->visitASTNode(decl);
        for (size_t i = 0; i < decl->numVarReferences(); i++)
        {
            VarReference *ref = (VarReference *)decl->varReference(i);

            auto table = decl->getParentScope()->scopeVarTable();
            auto name = ref->identifier()->name();

            if (table->lookup(name))
            {
                std::string message = "Redefinition of variable/parameter \"" + name + "\" in the same scope!";
                ErrorMessage error(message, ref->srcLoc());
                Errors.emplace_back(error);
            }
            Type type(decl->declType(), ref->isArray());
            VarSymbolEntry entry(type);
            table->insert(name, entry);
        }
    }

    static void checkParameters(std::vector<ErrorMessage> &Errors, FuncSymbolEntry *entry, FuncDeclaration *func)
    {
        auto name = func->name();
        if (func->numParameters() != entry->ParameterTypes.size())
        {
            std::string message = "Definition of function \"" + name + "()\" with different number of parameters!";
            ErrorMessage error(message, func->srcLoc());
            Errors.emplace_back(error);
        }
        else
        {
            for (size_t i = 0; i < func->numParameters(); i++)
            {
                auto parameter = func->parameter(i);
                if (entry->ParameterTypes[i] != parameter->type())
                {
                    std::string message = "Definition of function \"" + name + "()\" with different parameter type at position " + std::to_string(i) + "!";
                    ErrorMessage error(message, parameter->srcLoc());
                    Errors.emplace_back(error);
                }
            }
        }
    }

    void VerifyAndBuildSymbols::visitFuncDecl(FuncDeclaration *func)
    {
        // start your code here
        //  TODO: check if any path can cause segfault
        FuncSymbolTable *funcTable = func->root()->funcTable();
        // Hint: Check return type of the function does not match with each other
        //       Check number of parameters should match with each other
        //       Check each parameter type should match with each other
        //       Check there should be only one definition of the function
        //  can be declared but cannot be defined again
        auto name = func->name();
        auto entry = funcTable->lookup(name);
        if (entry != nullptr)
        {
            if (entry->HasBody && func->hasBody())
            {
                std::string message = "Redefinition of function \"" + name + "()\"";
                ErrorMessage error(message, func->srcLoc());
                Errors.emplace_back(error);
            }
            if (func->returnType() != entry->ReturnType)
            {
                std::string message = "Definition of function \"" + name + "()\" with different return type!";
                ErrorMessage error(message, func->srcLoc());
                Errors.emplace_back(error);
            }
            checkParameters(Errors, entry, func);
            entry->HasBody = true;
        }

        //      Check parameters cannot have the same name
        std::set<std::string> names;
        for (size_t i = 0; i < func->numParameters(); i++)
        {
            Parameter *param = func->parameter(i);
            if (names.find(param->name()) != names.end())
            {
                std::string message = "Redefinition of variable/parameter \"" + param->name() + "\"in the same scope!";
                ErrorMessage error(message, func->srcLoc());
                Errors.emplace_back(error);
            }
        }
        //      Check the last statement a function body must be return if the return type is not void
        if (func->returnType() != Type(Type::PrimitiveTypeEnum::Void) && func->hasBody())
        {
            ScopeStatement *body = func->body();
            // last child of the body should be a return statement
            ASTNode *lastNode = body->getChild(body->numChildren() - 1);
            if (!lastNode->isReturn())
            {
                std::string message = "The function \"" + func->name() + "()\" need to return a value at its end!";
                // ErrorMessage error("Function has non-void return type, but the return statement has no returned expression!", func->srcLoc());
                ErrorMessage error(message, func->srcLoc());
                Errors.emplace_back(error);
            }
        }

        if (entry == nullptr)
        {
            if (func->hasBody())
            {
                ScopeStatement *body = func->body();
                VarSymbolTable *table = body->scopeVarTable();
                for (size_t i = 0; i < func->numParameters(); i++)
                {
                    auto parameter = func->parameter(i);
                    VarSymbolEntry entry(parameter->type());
                    table->insert(parameter->name(), entry);
                }
            }
            std::vector<Type> paraTypes;
            for (size_t i = 0; i < func->numParameters(); i++)
            {
                // Add parameters to the scope
                auto parameter = func->parameter(i);
                paraTypes.push_back(parameter->type());
            }
            FuncSymbolEntry entry(func->returnType(), paraTypes, func->hasBody());
            funcTable->insert(name, entry);
        }
        this->visitASTNode(func);
    }

    void VerifyAndBuildSymbols::visitIfStmt(IfStatement *stmt)
    {
        // start your code here
        this->visitASTNode(stmt);
        // Hint: Check the conditional expression must have bool type
        Expr *expr = stmt->condExpr();
        if (expr->exprType() != Type(Type::PrimitiveTypeEnum::Bool))
        {
            std::string message = "Conditional expression in if statement has non-bool type!";
            ErrorMessage error(message, expr->srcLoc());
            Errors.emplace_back(error);
        }
    }

    void VerifyAndBuildSymbols::visitForStmt(ForStatement *stmt)
    {
        // start your code here
        this->visitASTNode(stmt);
        // Hint: Check the second expression in for must be either null or bool type
        Expr *expr = stmt->condExpr();
        if (expr != nullptr && expr->exprType() != Type(Type::PrimitiveTypeEnum::Bool))
        {
            std::string message = "Conditional expression in for statement has non-bool type!";
            ErrorMessage error(message, expr->srcLoc());
            Errors.emplace_back(error);
        }
    }

    void VerifyAndBuildSymbols::visitWhileStmt(WhileStatement *stmt)
    {
        // start your code here
        this->visitASTNode(stmt);
        // Hint: Check the conditional expression must have bool type
        Expr *expr = stmt->condExpr();
        if (expr != nullptr && expr->exprType() != Type(Type::PrimitiveTypeEnum::Bool))
        {
            std::string message = "Conditional expression in while statement has non-bool type!";
            ErrorMessage error(message, expr->srcLoc());
            Errors.emplace_back(error);
        }
    }

    void VerifyAndBuildSymbols::visitReturnStmt(ReturnStatement *stmt)
    {
        // start your code here
        this->visitASTNode(stmt);
        // Hint: Check void function must have no expression to return
        FuncDeclaration *parent = stmt->getParentFunction();
        if (parent->returnType() == Type(Type::Void) && stmt->hasReturnExpr())
        {
            std::string message = "Function has void return type, but the return statement has a returned expression!";
            ErrorMessage error(message, stmt->srcLoc());
            Errors.emplace_back(error);
        }
        //      Check Non-Void function must have an expression to return
        if (parent->returnType() != Type(Type::Void) && !stmt->hasReturnExpr())
        {
            // TODO: is this right?
            std::string message = "Function has non-void return type, but the return statement has no returned expression!";
            ErrorMessage error(message, stmt->srcLoc());
            Errors.emplace_back(error);
        }
        //      Check the return type and the returned expression type must match
        if (parent->returnType() != Type(Type::Void)) {
            Expr *expr = stmt->returnExpr();
            // TODO: array index issue
            if (parent->returnType().primitiveType() != expr->exprType().primitiveType())
            {
                std::string message = "Function has return type \"" + Type::typeToString(parent->returnType().primitiveType()) + "\"" +
                                      ", but the returned expression has type " + "\"" + Type::typeToString(expr->exprType().primitiveType()) + "\"!";
                ErrorMessage error(message, stmt->srcLoc());
                Errors.emplace_back(error);
            }
        }
    }

    void VerifyAndBuildSymbols::visitBreakStmt(BreakStatement *stmt)
    {
        // start your code here
        this->visitASTNode(stmt);
        // Hint: Check Break statement must appear inside a for/while statement
        auto forParent = stmt->getParentForStatement();
        auto whileParent = stmt->getParentWhileStatement();
        // if (forParent == nullptr && whileParent == nullptr) {
        //     std::string message = "Break statement must appear inside a for/while statement!";
        //     ErrorMessage error(message, stmt->srcLoc());
        //     Errors.emplace_back(error);
        // }
        // TODO: revert this?
        if (forParent == nullptr && whileParent == nullptr)
        {
            std::string message = "Break statement must appear inside a for statement!";
            ErrorMessage error(message, stmt->srcLoc());
            Errors.emplace_back(error);
        }
    }

    void VerifyAndBuildSymbols::visitUnaryExpr(UnaryExpr *expr)
    {
        // start your code here
        this->visitASTNode(expr);
        //      Check Not opcode must have bool operand
        auto opcode = expr->opcode();
        auto child = (Expr *)expr->getChild(0);
        if (opcode == Expr::Sub)
        {
            expr->setExprType(Type(Type::Int));
        }
        if (opcode == Expr::Not)
        {
            expr->setExprType(Type(Type::Bool));
        }
        if (opcode == Expr::ExprOpcode::Not && child->exprType().primitiveType() != Type::PrimitiveTypeEnum::Bool)
        {
            ErrorMessage error("Not \"!\" opcode must have bool operand!", expr->srcLoc());
            Errors.emplace_back(error);
        }
        // Hint: Check Negate opcode must have int operand!
        if (opcode == Expr::ExprOpcode::Sub && child->exprType().primitiveType() != Type::PrimitiveTypeEnum::Int)
        {
            ErrorMessage error("Negate \"-\" opcode must have int operand!", expr->srcLoc());
            Errors.emplace_back(error);
        }
    }
    void VerifyAndBuildSymbols::visitBinaryExpr(BinaryExpr *expr)
    {
        // start your code here
        this->visitASTNode(expr);
        // Hint: Check that for logical opcode, both operand need to be bool
        auto opcode = expr->opcode();
        auto expr1 = (Expr *)expr->getChild(0);
        auto expr2 = (Expr *)expr->getChild(1);

        bool hasError = false;

        if ((opcode == Expr::ExprOpcode::And || opcode == Expr::Or) &&
            (expr1->exprType().primitiveType() != Type::Bool ||
             expr2->exprType().primitiveType() != Type::Bool))
        {
            std::string message = "\"&&\"/\"||\" opcode must have bool operand!";
            ErrorMessage error(message, expr->srcLoc());
            Errors.emplace_back(error);
            hasError = true;
        }
        // Check that for equal and not equal opcode, both operand need to be the same primitive types
        if ((opcode == Expr::Equal || opcode == Expr::NotEqual) &&
            (expr1->exprType() != expr2->exprType()))
        {
            std::string message = "\"==\"/\"!=\" opcode must have same primitive type operand!";
            ErrorMessage error(message, expr->srcLoc());
            Errors.emplace_back(error);
            hasError = true;
        }
        // Check that for arithmetic and other comparison operand, both operand need to be int
        if ((opcode == Expr::Less || opcode == Expr::LessEqual || opcode == Expr::Greater || opcode == Expr::GreaterEqual ||
             opcode == Expr::Add || opcode == Expr::Sub || opcode == Expr::Mul || opcode == Expr::Div) &&
            (expr1->exprType().primitiveType() != Type::Int || expr2->exprType().primitiveType() != Type::Int))
        {
            auto operand = Expr::opcodeToString(opcode);
            std::string message = "\"" + operand + "\"" + " opcode must have int type operand!";
            ErrorMessage error(message, expr->srcLoc());
            Errors.emplace_back(error);
            hasError = true;
        }

        if (hasError)
        {
            expr->setExprType(Type(Type::Void));
        }
        else
        {
            if (opcode == Expr::Add || opcode == Expr::Sub || opcode == Expr::Div || opcode == Expr::Mul)
                expr->setExprType(Type(Type::Int));
            else
                expr->setExprType(Type(Type::Bool));
        }
    }

    void VerifyAndBuildSymbols::visitCallExpr(CallExpr *expr)
    {
        // start your code here
        // Hint: Check Call undeclared function
        //       Check the number of arguments must match the number of parameters
        //       Check the type of each parameter must match the argument
        this->visitASTNode(expr);
        auto table = expr->root()->funcTable();
        auto name = expr->callee()->name();
        FuncSymbolEntry *entry = table->lookup(name);
        if (entry == nullptr)
        {
            std::string message = "Function \"" + name + "()\" is not declared before use!";
            ErrorMessage error(message, expr->srcLoc());
            Errors.emplace_back(error);
            return;
        }

        expr->setExprType(entry->ReturnType);

        if (expr->numArgs() != entry->ParameterTypes.size())
        {
            // Function name() is declared with x parameters but called with y arguments!
            std::string message = "Function \"" + name + "()\" is declared with " + std::to_string(entry->ParameterTypes.size()) +
                                  " parameters but called with " + std::to_string(expr->numArgs()) + " arguments!";
            ErrorMessage error(message, expr->srcLoc());
            Errors.emplace_back(error);
            return;
        }
        for (size_t i = 0; i < expr->numArgs(); i++)
        {
            // TODO: if array,check we are indexing into an array
            if (entry->ParameterTypes[i] != expr->arg(i)->exprType())
            {
                std::string message = "Function \"" + name + "()\" does not match the type of the call argument at position " + std::to_string(i) + "!";
                ErrorMessage error(message, expr->srcLoc());
                Errors.emplace_back(error);
            }
        }
    }

    static Type verifyVarReference(std::vector<ErrorMessage> &Errors, Expr *expr, VarReference *ref)
    {
        // start your code here
        // Hint: Check the vairable which is reference must be declared before
        auto name = ref->identifier()->name();
        auto table = ref->locateDeclaringTableForVar(name);
        VarSymbolEntry *entry = nullptr;
        if (table)
        {
            entry = table->lookup(name);
        }
        if (entry == nullptr)
        {
            std::string message = "Variable " + name + " is not declared before use!";
            ErrorMessage error(message, expr->srcLoc());
            Errors.emplace_back(error);
        }
        else
        {
            //      Check index expression must have int type
            if (ref->isArray())
            {
                auto index = ref->indexExpr();
                if (index != nullptr && index->exprType().primitiveType() != Type::Int)
                {
                    std::string message = "Array index expressions must have int operand!";
                    ErrorMessage error(message, expr->srcLoc());
                    Errors.emplace_back(error);
                }
                //      Check variable must be declared as an array for indexing
                auto type = entry->VarType;
                if (type.arrayBound() <= 0 && index)
                {
                    std::string message = "Indexing an non-array variable!";
                    ErrorMessage error(message, expr->srcLoc());
                    Errors.emplace_back(error);
                }
            }
        }
        // return ref Type
        if (entry)
        {
            return entry->VarType;
        }
        return Type(Type::Void);
    }

    void VerifyAndBuildSymbols::visitVarExpr(VarExpr *expr)
    {
        // start your code here
        // Hint: invoke verifyVarReference to verify
        this->visitASTNode(expr);
        Type varType = verifyVarReference(Errors, expr, (VarReference *)expr->getChild(0));
        expr->setExprType(varType);
    }

    void VerifyAndBuildSymbols::visitAssignmentExpr(AssignmentExpr *expr)
    {
        // start your code here
        // Hint: invoke verifyVarReference to verify
        //       Also, check var and assigned expression must have the same type
        this->visitASTNode(expr);
        Type type = verifyVarReference(Errors, expr, (VarReference *)expr->getChild(0));
        Expr *value = (Expr *)expr->getChild(1);

        // TODO: edge case for array indexing
        // a[10] = 0
        // a = 0
        // would it work if we check the primitive type like below? Or are we missing something else
        // by doing this?
        if (type.primitiveType() != value->exprType().primitiveType())
        {
            std::string message = "Variable and the assignment expression do not have the same type!";
            ErrorMessage error(message, expr->srcLoc());
            Errors.emplace_back(error);
        }
    }

    void VerifyAndBuildSymbols::visitIntLiteralExpr(IntLiteralExpr *literal)
    {
        // start your code here
        // Hint: Check Integer literal must be inside the range of int
        this->visitASTNode(literal);
        int value = literal->value();
        if (value < INT32_MIN || value > INT32_MAX)
        {
            std::string message = "Integer literal must be inside the range of int!";
            ErrorMessage error(message, literal->srcLoc());
            Errors.emplace_back(error);
        }
    }

    void VerifyAndBuildSymbols::visitBoolLiteralExpr(BoolLiteralExpr *literal)
    {
        // start your code here
        this->visitASTNode(literal);
    }

    void VerifyAndBuildSymbols::visitCharLiteralExpr(CharLiteralExpr *literal)
    {
        // start your code here
        this->visitASTNode(literal);
    }

    // print collected error messages
    std::string VerifyAndBuildSymbols::genErrorMessages()
    {
        std::stringbuf buf;
        std::ostream os(&buf);

        for (size_t i = 0; i < Errors.size(); i++)
        {
            os << Errors[i].Msg << " (" << Errors[i].SrcLoc.Line << ":" << Errors[i].SrcLoc.Row << ")\n";
        }

        return buf.str();
    }

}