grammar MiniC;

@header {
#include <vector>
#include "Program.h"
#include "Declarations.h"
#include "Statements.h"
#include "Exprs.h"
#include "Terms.h"
}
/*Add your grammar rules in Assignment 2*/
/*Then add compiler actions in Assignment 3*/
prog returns [minicc::Program *val]
/*You may need init in Assignment 3*/
@init {
    // std::cerr << "[DEBUG] Entering prog" << std::endl;
    $val = new minicc::Program();
    $val->setParent(nullptr);
    $val->setRoot($val);
    $val->setSyslibFlag(false);
    int line = _ctx->start->getLine();
    int col = _ctx->start->getCharPositionInLine();
    $val->setSrcLoc(line, col);
}
@after {
    // std::cerr << "[DEBUG] Leaving prog" << std::endl;
}
:	(preamble | ) decls+=decl* EOF {
    if ($preamble.start && $preamble.headerIncluded) {
        $val->setSyslibFlag(true);
        // std::cerr << "[DEBUG] Preamble included" << std::endl;
    }
    for (auto decl: $decls) {
        $val->addChild(decl->val);
    }
}
;
preamble returns [bool headerIncluded]
:  '#include' '"minicio.h"' { 
        $headerIncluded = true;
    }
;
decl returns [minicc::Declaration *val]
@init {
    // std::cerr << "[DEBUG] Entering decl" << std::endl;
}
:   vardecl {
        $val = $vardecl.val;
    }
    |   rettype funcname '(' parameters ')' scope {
        auto decl = new minicc::FuncDeclaration();
        decl->setHasBody(true);
        $val = decl;
        $val->addChild($rettype.val);
        $val->addChild($funcname.val);
        for (auto param: *$parameters.val) {
            $val->addChild(param);
        }
        $val->addChild($scope.val);
    }
    |   rettype funcname '(' parameters ')' ';' {
        auto decl = new minicc::FuncDeclaration();
        decl->setHasBody(true);
        $val = decl;
        $val->addChild($rettype.val);
        $val->addChild($funcname.val);
        for (auto param: *$parameters.val) {
            $val->addChild(param);
        }
    }
    ;
vardecl returns [minicc::Declaration *val]
@init {
    // std::cerr << "[DEBUG] Entering vardecl" << std::endl;
}
@after {
    // std::cerr << "[DEBUG] Leaving vardecl" << std::endl;
}
:   vartype varlist ';' {
        $val = new minicc::VarDeclaration();
        $val->addChild($vartype.val);
            for (auto var : *$varlist.val) {
                $val->addChild(var);
            }
        int line = _ctx->start->getLine();
        int col = _ctx->start->getCharPositionInLine();
        $val->setSrcLoc(line, col);
    }
    ;
scope returns [minicc::ScopeStatement *val]
/*You may need init in Assignment 3*/
@init {
    // std::cerr << "[DEBUG] Entering scope" << std::endl;
}
@after {
    // std::cerr << "[DEBUG] Leaving scope" << std::endl;
}
:  '{' vardecls+=vardecl* stmts+=stmt* '}' {
        $val = new minicc::ScopeStatement();
        $val->setNumVarDecl($vardecls.size());
        for (auto vardecl: $vardecls) {
            $val->addChild(vardecl->val);
        }
        for (auto stmt: $stmts) {
            $val->addChild(stmt->val);
        }
    }
    ;
stmt returns [minicc::Statement *val]
:   expr ';' {
        $val = new minicc::ExprStatement();
        $val->addChild($expr.val);
    }
    |   'if' '(' expr ')' stmt {
        $val = new minicc::IfStatement();
        $val->addChild($expr.val);
        $val->addChild($stmt.val);
    }
    |   'if' '(' expr ')' s1=stmt 'else' s2=stmt {
        $val = new minicc::IfStatement();
        $val->addChild($expr.val);
        $val->addChild($s1.val);
        $val->addChild($s2.val);
    }
    |   'for' '(' e1=expropt ';' e2=expropt ';' e3=expropt ')' stmt {
        $val = new minicc::ForStatement();
        $val->addChild($e1.val);
        $val->addChild($e2.val);
        $val->addChild($e3.val);
        $val->addChild($stmt.val);
    }
    |   'while' '(' expr ')' stmt {
        $val = new minicc::WhileStatement();
        $val->addChild($expr.val);
        $val->addChild($stmt.val);
    }
    | 'break' ';' {
        $val = new minicc::BreakStatement();
    }
    | 'return' ';' {
        $val = new minicc::ReturnStatement();
    }
    |   'return' expr ';' {
        $val = new minicc::ReturnStatement();
        $val->addChild($expr.val);
    }
    |   scope {
        $val = $scope.val;
    }
    ;
varlist returns [std::vector<minicc::VarReference*> *val]
@init {
    // std::cerr << "[DEBUG] Entering varlist" << std::endl;
}
@after {
    // std::cerr << "[DEBUG] Leaving varlist" << std::endl;
}
:    varlistentry {
        $val = new std::vector<minicc::VarReference*>();
        $val->push_back($varlistentry.val);
    }
    |   v1=varlist ',' varlistentry {
        $val = $v1.val;
        $val->push_back($varlistentry.val);
    }
    ;
varlistentry returns [minicc::VarReference *val]
@init {
    $val = new minicc::VarReference();
    // std::cerr << "[DEBUG] Entering varlistentry" << std::endl;
}
@after {
    // std::cerr << "[DEBUG] Leaving varlistentry" << std::endl;
}
:   varname {
        $val->addChild($varname.val);
        
        int line = _ctx->start->getLine();
        int col = _ctx->start->getCharPositionInLine();
        $val->setSrcLoc(line, col);
    }
    |   varname '[' INT ']' {
        $val->addChild($varname.val);
        
        auto intNode = new minicc::IntLiteralExpr($INT.int);
        $val->addChild(intNode);
        
        int line = _ctx->start->getLine();
        int col = _ctx->start->getCharPositionInLine();
        $val->setSrcLoc(line, col);
    }
    ;
vartype returns [minicc::TypeReference *val]
:   t=('int' | 'bool' | 'char') {
        $val = new minicc::TypeReference(
            $t.text == "int"  ? minicc::Type::PrimitiveTypeEnum::Int  :
            $t.text == "bool" ? minicc::Type::PrimitiveTypeEnum::Bool :
                          minicc::Type::PrimitiveTypeEnum::Char
        );
    }
;
rettype returns [minicc::TypeReference *val]
:    l='void' {
        $val = new minicc::TypeReference(minicc::Type::PrimitiveTypeEnum::Void);
        $val->setSrcLoc($l.line, $l.pos);    
    }
    |   vartype {
        $val = $vartype.val;
    }
    ;
parameters returns [std::vector<minicc::Parameter*> *val]
:   parameterlist? {
        if ($parameterlist.start)
            $val = $parameterlist.val;
        else
            $val = new std::vector<minicc::Parameter*>();
    }
;
parameterlist returns [std::vector<minicc::Parameter*> *val]
@init {
    // std::cerr << "[DEBUG] Entering parameterlist" << std::endl;
}
@after {
    // std::cerr << "[DEBUG] Leaving parameterlist" << std::endl;
}
:   parameterentry {
        $val = new std::vector<minicc::Parameter*>();
        $val->push_back($parameterentry.val);
    }
    |   p1=parameterlist ',' parameterentry {
        $val = $p1.val;
        $val->push_back($parameterentry.val);
    }
    ;
parameterentry returns [minicc::Parameter *val]
:   vartype parametername {
        $val = new minicc::Parameter();
        $val->addChild($vartype.val);
        $val->addChild($parametername.val);
    }
    ;
expropt returns [minicc::Expr *val]
:    expr {
        $val = $expr.val;
    }
    |   /* eps */
    ;
expr returns [minicc::Expr* val]
:   INT {
        $val = new minicc::IntLiteralExpr($INT.int);
    }
    |   CHAR {
        $val = new minicc::CharLiteralExpr($CHAR.text[1]);
    }
    |   op='-' expr {
        auto expr = new minicc::UnaryExpr();
        expr->setOpcode(expr->opcodeFromString($op.text));
        expr->addChild($expr.val);
        $val = expr;
    }
    |   e1=expr op=('*'|'/') e2=expr {
        auto expr = new minicc::BinaryExpr();
        expr->setOpcode(expr->opcodeFromString($op.text));
        expr->addChild($e1.val);
        expr->addChild($e2.val);
        $val = expr;
    }
    |   e1=expr op=('+'|'-') e2=expr {
        auto expr = new minicc::BinaryExpr();
        expr->setOpcode(expr->opcodeFromString($op.text));
        expr->addChild($e1.val);
        expr->addChild($e2.val);
        $val = expr;
    }
    |   e1=expr op=('==' | '!=' | '<' | '<=' | '>' | '>=') e2=expr {
        auto expr = new minicc::BinaryExpr();
        expr->setOpcode(expr->opcodeFromString($op.text));
        expr->addChild($e1.val);
        expr->addChild($e2.val);
        $val = expr;
    }
    |   op='!' expr {
        auto expr = new minicc::UnaryExpr();
        expr->setOpcode(expr->opcodeFromString($op.text));
        expr->addChild($expr.val);
        $val = expr;
    }
    |   e1=expr op='&&' e2=expr {
        auto expr = new minicc::BinaryExpr();
        expr->setOpcode(expr->opcodeFromString($op.text));
        expr->addChild($e1.val);
        expr->addChild($e2.val);
        $val = expr;
    }
    |   e1=expr op='||' e2=expr {
        auto expr = new minicc::BinaryExpr();
        expr->setOpcode(expr->opcodeFromString($op.text));
        expr->addChild($e1.val);
        expr->addChild($e2.val);
        $val = expr;
    }
    |   var '=' expr {
        auto expr = new minicc::AssignmentExpr();
        expr->addChild($var.val);
        expr->addChild($expr.val);
        $val = expr;
    }
    | v=('true' | 'false') {
        bool value = $v.text == "true";
        auto expr = new minicc::BoolLiteralExpr(value);
        $val = expr;
    }
    |   '(' expr ')' {
        $val = $expr.val;
    }
    |   var {
        $val = new minicc::VarExpr();
        $val->addChild($var.val);
    }
    |   funcname '(' arguments ')' {
        $val = new minicc::CallExpr();
        $val->addChild($funcname.val);
        if ($arguments.start) {
            for (auto arg: *$arguments.val) {
                $val->addChild(arg);
            }
        }
    }
    |   parametername {
        $val = new minicc::VarExpr();
        auto reference = new minicc::VarReference();
        reference->addChild($parametername.val);
        $val->addChild(reference);
    }
    ;
var returns [minicc::VarReference *val]
@init {
    $val = new minicc::VarReference();
}
:    varname {
        $val->addChild(new minicc::Identifier($varname.text));
    }
    |   varname '[' expr ']' {
        $val->addChild(new minicc::Identifier($varname.text));
        $val->addChild($expr.val);
        $expr.val->setParent($val);
    }
    ;
arguments returns [std::vector<minicc::Expr*> *val]
:   /* epsilon */
    |   argumentlist {
        $val = $argumentlist.val;
    }
    ;
argumentlist returns [std::vector<minicc::Expr*> *val]
@init {
    $val = new std::vector<minicc::Expr*>();
}
:   expr {
        $val->push_back($expr.val);
    }
    |   a1=argumentlist ',' expr {
        $val = $a1.val;
        $val->push_back($expr.val);
    }
    ;
varname returns [minicc::Identifier *val]
:   ID {
        $val = new minicc::Identifier($ID.text);
    }
    ;
funcname returns [minicc::Identifier *val]
:   ID {
        $val = new minicc::Identifier($ID.text);
    }
    ;
parametername returns [minicc::Identifier *val]
:   ID {
        $val = new minicc::Identifier($ID.text);
    }
    ;

ID:     [a-zA-Z][a-zA-Z0-9_]* ;
INT:    [0] | ([1-9][0-9]*) ;
CHAR:   '\'' . '\'' ;
WS:     [ \t\r\n]+ -> skip;
COMMENT: '//' (~[\r\n])* -> skip;
