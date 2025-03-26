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
                    child->setRoot((Program *)node);
                else
                    child->setRoot(node->root());
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
            FuncSymbolEntry getintEntry(Type(Type::Int), std::vector<Type>(), true);
            funcTable->insert("getint", getintEntry);

            std::vector<Type> putintParams;
            putintParams.push_back(Type(Type::Int));
            FuncSymbolEntry putintEntry(Type(Type::Void), putintParams, true);
            funcTable->insert("putint", putintEntry);

            std::vector<Type> putcharParams;
            putcharParams.push_back(Type(Type::Char));
            FuncSymbolEntry putcharacterEntry(Type(Type::Void), putcharParams, true);
            funcTable->insert("putcharacter", putcharacterEntry);

            FuncSymbolEntry putnewlineEntry(Type(Type::Void), std::vector<Type>(), true);
            funcTable->insert("putnewline", putnewlineEntry);
        }
        VisitingProgram = prog;
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
            Type type;
            if (ref->isArray()) {
                auto *bound = dynamic_cast<IntLiteralExpr*>(ref->indexExpr());
                type = Type(decl->declType(), bound->value());
            } else {
                type = Type(decl->declType());
            }
            VarSymbolEntry entry(type);
            table->insert(name, entry);
        }
    }

    static bool checkMismatchedParameterType(std::vector<ErrorMessage> &Errors, FuncSymbolEntry *entry, FuncDeclaration *func)
    {
        bool pass = true;
        for (size_t i = 0; i < func->numParameters(); i++)
        {
            auto parameter = func->parameter(i);
            if (entry->ParameterTypes[i] != parameter->type())
            {
                std::string message = "Definition of function \"" + func->name() + "()\" with different parameter type at position " + std::to_string(i) + "!";
                ErrorMessage error(message, parameter->srcLoc());
                Errors.emplace_back(error);
                pass = false;
            }
        }
        return pass;
    }

    // Check the parameters in entry and func match, i.e. they have the same length and same types
    static bool checkParameterCount(std::vector<ErrorMessage> &Errors, FuncSymbolEntry *entry, FuncDeclaration *func)
    {
        auto name = func->name();
        if (func->numParameters() != entry->ParameterTypes.size())
        {
            std::string message = "Definition of function \"" + name + "()\" with different number of parameters!";
            ErrorMessage error(message, func->srcLoc());
            Errors.emplace_back(error);
            return false;
        }
        return true;
    }

    static bool checkReturnType(std::vector<ErrorMessage> &Errors, FuncSymbolEntry *entry, FuncDeclaration *func)
    {
        if (func->returnType() != entry->ReturnType)
        {
            std::string message = "Definition of function \"" + func->name() + "()\" with different return type!";
            ErrorMessage error(message, func->srcLoc());
            Errors.emplace_back(error);
            return false;
        }
        return true;
    }

    static bool checkFuncRedefinition(std::vector<ErrorMessage> &Errors, FuncSymbolEntry *entry, FuncDeclaration *func)
    {
        if (entry->HasBody && func->hasBody())
        {
            std::string message = "Redefinition of function \"" + func->name() + "()\"!";
            ErrorMessage error(message, func->srcLoc());
            Errors.emplace_back(error);
            return false;
        }
        return true;
    }

    static bool checkParamRedefinition(std::vector<ErrorMessage> &Errors, FuncDeclaration *func)
    {
        std::set<std::string> names;
        bool pass = true;
        for (size_t i = 0; i < func->numParameters(); i++)
        {
            Parameter *param = func->parameter(i);
            if (names.find(param->name()) != names.end())
            {
                std::string message = "Redefinition of variable/parameter \"" + param->name() + "\"in the same scope!";
                ErrorMessage error(message, func->srcLoc());
                Errors.emplace_back(error);
                pass = false;
            }
            names.insert(param->name());
        }
        return pass;
    }

    static bool checkReturnStatement(std::vector<ErrorMessage> &Errors, FuncDeclaration *func)
    {
        if (func->returnType() != Type(Type::Void) && func->hasBody())
        {
            ScopeStatement *body = func->body();
            if (body->numChildren() == 0 || !(body->getChild(body->numChildren() - 1)->isReturn()))
            {
                ErrorMessage error("The function \"" + func->name() + "()\" need to return a value at its end!", func->srcLoc());
                Errors.emplace_back(error);
                return false;
            }
        }
        return true;
    }

    static void insertFunc(FuncSymbolTable *funcTable, FuncSymbolEntry *existingEntry, FuncDeclaration *func)
    {
        if (existingEntry)
        {
            if (func->hasBody())
                existingEntry->HasBody = true;
            return;
        }
        // Insert an entry here
        ScopeStatement *body;
        VarSymbolTable *table;
        if (func->hasBody())
        {
            body = func->body();
            table = body->scopeVarTable();
        }

        std::vector<Type> paraTypes;
        for (size_t i = 0; i < func->numParameters(); i++)
        {
            // Add parameters to the function symbol table
            auto parameter = func->parameter(i);
            paraTypes.push_back(parameter->type());

            // Add parameters to the function scope
            if (func->hasBody())
            {
                VarSymbolEntry entry(parameter->type());
                table->insert(parameter->name(), entry);
            }
        }
        FuncSymbolEntry entry(func->returnType(), paraTypes, func->hasBody());
        funcTable->insert(func->name(), entry);
    }

    void VerifyAndBuildSymbols::visitFuncDecl(FuncDeclaration *func) {
        Program* root = VisitingProgram;
        Type t = func->returnType();
        std::string name = func->name();
        std::vector<Type> parameterTypes;
        std::vector<std::string> parameterNames;
        parameterTypes.clear();
        parameterNames.clear();
        for (size_t i = 0; i < func->numParameters(); i++) {
            Parameter* para = func->parameter(i);
            std::string name = para->name();
            parameterTypes.push_back(para->type());
            parameterNames.push_back(para->name());
        }
        // If we already have this function, we are going to check the type matches with each other.
        if (FuncSymbolEntry *entry = root->funcTable()->lookup(name)) {
            // Check2: Return type of the function does not match with each other
            if (entry->ReturnType != t)
                Errors.push_back(ErrorMessage("Definition of function \"" + name + "()\" with different return type!", func->srcLoc()));
            // Check3: Number of parameters should match with each other
            if (entry->ParameterTypes.size() != parameterTypes.size())
                Errors.push_back(ErrorMessage("Definition of function \"" + name + "()\" with different number of parameters!", func->srcLoc()));
            // Check4: Each parameter type should match with each other
            for (size_t i = 0; i < std::min(parameterTypes.size(), entry->ParameterTypes.size()); i++)
                if (entry->ParameterTypes[i] != parameterTypes[i])
                    Errors.push_back(ErrorMessage("Definition of function \"" + name + "()\" with different parameter type at position " + std::to_string(i) + "!", func->srcLoc()));
            // Check5: Only one definition of the function.
            if (func->hasBody())
                if (entry->HasBody)
                    Errors.push_back(ErrorMessage("Redefinition of function \"" + name + "()\"!", func->srcLoc()));
        }
        else
            root->funcTable()->insert(name, FuncSymbolEntry(t, parameterTypes, false));
        if (func->hasBody()) {
            auto entry = root->funcTable()->lookup(name);
            entry->HasBody = true;
            ScopeStatement* body = func->body();
            // Check6: Parameters cannot have the same name
            for (size_t i = 0; i < parameterTypes.size(); i++)
                if (body->scopeVarTable()->lookup(parameterNames[i]))
                    Errors.push_back(ErrorMessage("Redefinition of variable/parameter \"" + parameterNames[i] + "\" in the same scope!", func->parameter(i)->getChild(0)->srcLoc()));
                else {
                    auto table = body->scopeVarTable();
                    table->insert(parameterNames[i], VarSymbolEntry(parameterTypes[i]));
                }
        }
        this->visitASTNode(func);
        if (func->hasBody()) {
            // Check26: The last statement a function body must be return if the return type is not void
            ScopeStatement* body = func->body();
            Statement* lastStatement = nullptr;
            if (body->numChildren() > 0)
                lastStatement = (Statement*) body->getChild(body->numChildren() - 1);
            if (!func->returnType().isVoid() && (lastStatement == nullptr || !lastStatement->isReturn()))
                Errors.push_back(ErrorMessage("The function \"" + name + "()\" need to return a value at its end!", func->srcLoc()));
        }
    }

    void VerifyAndBuildSymbols::visitIfStmt(IfStatement *stmt)
    {
        // start your code here
        this->visitASTNode(stmt);
        // Hint: Check the conditional expression must have bool type
        Expr *expr = stmt->condExpr();
        if (expr->exprType() != Type(Type::Bool))
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
        Expr *expr = stmt->condExpr();
        if (expr != nullptr && expr->exprType() != Type(Type::Bool))
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
        if (expr != nullptr && expr->exprType() != Type(Type::Bool))
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
            std::string message = "Function has non-void return type, but the return statement has no returned expression!";
            ErrorMessage error(message, stmt->srcLoc());
            Errors.emplace_back(error);
        }
        //      Check the return type and the returned expression type must match
        if (parent->returnType() != Type(Type::Void))
        {
            Expr *expr = stmt->returnExpr();
            if (parent->returnType() != expr->exprType())
            {
                std::string message = "Function has return type \"" + parent->returnType().toString() + "\"" +
                                      ", but the returned expression has type " + "\"" + expr->exprType().toString() + "\"!";
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
        if (forParent == nullptr && whileParent == nullptr)
        {
            std::string message = "Break statement must appear inside a for/while statement!";
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
            expr->setExprType(child->exprType());
        }
        if (opcode == Expr::Not)
        {
            expr->setExprType(child->exprType());
        }
        if (opcode == Expr::ExprOpcode::Not && (child->exprType() != Type(Type::Bool)))
        {
            ErrorMessage error("Not \"!\" opcode must have bool operand!", expr->srcLoc());
            Errors.emplace_back(error);
        }
        // Hint: Check Negate opcode must have int operand!
        if (opcode == Expr::ExprOpcode::Sub && (child->exprType() != Type(Type::Int)))
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

        Type boolType(Type::Bool);
        Type intType(Type::Int);
        Type voidType(Type::Void);

        if ((opcode == Expr::ExprOpcode::And || opcode == Expr::Or) &&
            (expr1->exprType() != boolType ||
             expr2->exprType() != boolType))
        {
            std::string message = "\"&&\"/\"||\" opcode must have bool operand!";
            ErrorMessage error(message, expr->srcLoc());
            Errors.emplace_back(error);
        }
        // Check that for equal and not equal opcode, both operand need to be the same primitive types
        if ((opcode == Expr::Equal || opcode == Expr::NotEqual) &&
            (expr1->exprType() != expr2->exprType()))
        {
            std::string message = "\"==\"/\"!=\" opcode must have same primitive type operand!";
            ErrorMessage error(message, expr->srcLoc());
            Errors.emplace_back(error);
        }
        // Check that for arithmetic and other comparison operand, both operand need to be int
        if ((Expr::isArithmeticComparison(opcode) || Expr::isArithmetic(opcode)) &&
            (expr1->exprType() != intType || expr2->exprType() != intType))
        {
            auto operand = Expr::opcodeToString(opcode);
            std::string message = "\"" + operand + "\"" + " opcode must have int type operand!";
            ErrorMessage error(message, expr->srcLoc());
            Errors.emplace_back(error);
        }

        // ==, !=, >, <, >=, <= always has boolean types
        // Other operators inherit type of the first operand

        if (Expr::isComparison(opcode))
            expr->setExprType(boolType);
        else
            expr->setExprType(expr1->exprType());
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
            std::string message = "Function " + name + "() is not declared before use!";
            ErrorMessage error(message, expr->srcLoc());
            Errors.emplace_back(error);
            return;
        }

        expr->setExprType(entry->ReturnType);

        if (expr->numArgs() != entry->ParameterTypes.size())
        {
            std::string message = "Function " + name + "() is declared with " + std::to_string(entry->ParameterTypes.size()) +
                                  " parameters but called with " + std::to_string(expr->numArgs()) + " arguments!";
            ErrorMessage error(message, expr->srcLoc());
            Errors.emplace_back(error);
            return;
        }
        for (size_t i = 0; i < expr->numArgs(); i++)
        {
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
        // Referencing undefined variable -> return void type
        // Indexing non-array type, result should be the same type as type
        auto name = ref->identifier()->name();
        auto table = ref->locateDeclaringTableForVar(name);
        VarSymbolEntry *entry = nullptr;
        if (table)
            entry = table->lookup(name);
        if (entry == nullptr)
        {
            std::string message = "Variable " + name + " is not declared before use!";
            ErrorMessage error(message, expr->srcLoc());
            Errors.emplace_back(error);
            return Type(Type::Void);
        }
        else
        {
            //      Check index expression must have int type
            if (ref->isArray())
            {
                auto index = ref->indexExpr();
                if (index != nullptr && index->exprType() != Type(Type::Int))
                {
                    std::string message = "Array index expressions must have int operand!";
                    ErrorMessage error(message, expr->srcLoc());
                    Errors.emplace_back(error);
                }
                //      Check variable must be declared as an array for indexing
                auto type = entry->VarType;
                if (type.arrayBound() <= 0)
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
            // If indexing an array, return the primitive type of the array
            if (ref->isArray())
            {
                return Type(entry->VarType.primitiveType());
            }
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

        if (type != value->exprType())
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
        if (literal->exprType() == Type(Type::Void))
        {
            std::string message = "Integer literal must be inside the range of int!";
            ErrorMessage error(message, literal->srcLoc());
            Errors.emplace_back(error);
            literal->setExprType(Type(Type::Int));
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