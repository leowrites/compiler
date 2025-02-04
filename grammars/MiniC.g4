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
    $val = new minicc::Program();   
}
:	preamble decl {
        if ($preamble.headerIncluded) {
            $val->setSyslibFlag(true);
        }
        $val->addChild($decl.val);
    }
;
preamble returns [bool headerIncluded]
:  '#include' '"minicio.h"' { $headerIncluded = true; } 
    | { $headerIncluded = false; } 
;
decl returns [minicc::Declaration *val]
:   vardecl {
        $val = new minicc::VarDeclaration();
        for (auto d: *$vardecl.val) {
            $val->addChild(d);
        }    
    }
    | rettype funcname '(' parameters ')' scope {
        $val = new minicc::FuncDeclaration();
        $val->addChild($rettype.val);
        $val->addChild($funcname.val);
        for (auto param: *$parameters.val) {
            $val->addChild(param);
        }
    }
    | rettype funcname '(' parameters ')' ';' {
        $val = new minicc::FuncDeclaration();
        $val->addChild($rettype.val);
        $val->addChild($funcname.val);
        for (auto param: *$parameters.val) {
            $val->addChild(param);
        }
    }
    | d1=decl d2=decl {
        $val = new minicc::MultipleDeclaration();
        $val->addChild($d1.val);
        $val->addChild($d2.val);
    }
    ;

vardecl returns [std::vector<minicc::VarDeclaration*> *val]
@init {
    $val = new std::vector<minicc::VarDeclaration*>();
}
:   vartype varlist ';' {
        auto varDecl = new minicc::VarDeclaration();
        varDecl->addChild($vartype.val);
        for (minicc::VarReference* ref: *$varlist.val) {
           varDecl->addChild(ref);
        }
        $val->emplace_back(varDecl);
    }
    | v1=vardecl v2=vardecl {
        for (auto v: *$v1.val) {
            $val->emplace_back(v);
        }
        for (auto v: *$v2.val) {
            $val->emplace_back(v);
        }
    }
    ;
scope returns [minicc::ScopeStatement *val]
/*You may need init in Assignment 3*/
@init {
    $val = new minicc::ScopeStatement();
}
:  '{' vardecl stmt_seq '}' {
        for (auto vardecl: *$vardecl.val) {
            $val->addChild(vardecl);
        }
        $val->addChild($stmt_seq.val);
    }
    | '{' stmt_seq '}' {
        $val->addChild($stmt_seq.val);
    }
    | '{' '}'
    ;

stmt_seq returns [minicc::StatementSequence *val]
@init {
    $val = new minicc::StatementSequence();
}
: s+=stmt+ {
      for (auto stmtNode : $s) {
          $val->addChild(stmtNode->val);
      }
}
;
stmt returns [minicc::Statement *val]
:   expr ';' {
        $val = new minicc::ExprStatement();
        $val->addChild($expr.val);
    }
    | 'if' '(' expr ')' stmt {
        $val = new minicc::IfStatement();
        $val->addChild($expr.val);
        $val->addChild($stmt.val);
    }
    | 'if' '(' expr ')' s1=stmt 'else' s2=stmt {
        $val = new minicc::IfStatement();
        $val->addChild($expr.val);
        $val->addChild($s1.val);
        $val->addChild($s2.val);
    }
    | 'for' '(' e1=expropt ';' e2=expropt ';' e3=expropt ')' stmt_seq {
        $val = new minicc::ForStatement();
        $val->addChild($e1.val);
        $val->addChild($e2.val);
        $val->addChild($e3.val);
        $val->addChild($stmt_seq.val);
    }
    | 'while' '(' expr ')' stmt_seq {
        $val = new minicc::WhileStatement();
        $val->addChild($expr.val);
        $val->addChild($stmt_seq.val);
    }
    | 'break' ';' {
        $val = new minicc::BreakStatement();
    }
    | 'return' ';' {
        $val = new minicc::ReturnStatement();
    }
    | 'return' expr ';' {
        $val = new minicc::ReturnStatement();
        $val->addChild($expr.val);
    }
    | scope {
        $val = $scope.val;
    }
    ;

varlist returns [std::vector<minicc::VarReference*> *val]
@init {
    $val = new std::vector<minicc::VarReference*>();
}
:   varlistentry {
        $val->emplace_back($varlistentry.val);
    }
    | v1=varlist ',' varlistentry {
        $val = $v1.val;
        $val->emplace_back($varlistentry.val);
    }
    ;
varlistentry returns [minicc::VarReference *val]
:   varname {
        auto reference = new minicc::VarReference();
        reference->addChild($varname.val);
        $val = reference;
    }
    | varname '[' INT ']' {
        auto reference = new minicc::VarReference();
        reference->addChild($varname.val);
        reference->addChild(new minicc::IntLiteralExpr($INT.int));
        $val = reference;
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
:   'void' {
        $val = new minicc::TypeReference(minicc::Type::PrimitiveTypeEnum::Void);
    }
    | vartype {
        $val = $vartype.val;
    }
    ;

// vector of parameters
parameters returns [std::vector<minicc::Parameter*> *val]
:   parameterlist? {
        $val = $parameterlist.val;
    }
    ;

parameterlist returns [std::vector<minicc::Parameter*> *val]
@init {
    $val = new std::vector<minicc::Parameter*>();
}
:   vartype parametername {
        auto decl = new minicc::Parameter();
        decl->addChild($vartype.val);
        decl->addChild($parametername.val);
    }
    | v1=parameterlist ',' v2=parameterlist {
        for (auto param: *$v1.val) {
            $val->emplace_back(param);
        }
        for (auto param: *$v2.val) {
            $val->emplace_back(param);
        }
    }
    ;
parameterentry
:   
    ;
expropt returns [minicc::Expr* val]
:   expr? {
        $val = $expr.val;
    }
    | {
        $val = nullptr;
    }
    ;
expr returns [minicc::Expr* val]
:   INT {
        $val = new minicc::IntLiteralExpr($INT.int);
    }
    | CHAR {
        $val = new minicc::CharLiteralExpr($CHAR.text[1]);
    }
    | op='-' expr {
        auto expr = new minicc::UnaryExpr();
        expr->setOpcode(expr->opcodeFromString($op.text));
        $val = expr;
    }
    | op='!' expr {
        auto expr = new minicc::UnaryExpr();
        expr->setOpcode(expr->opcodeFromString($op.text));
        $val = expr;
    }
    | expr op=('*' | '/') expr {
        auto expr = new minicc::BinaryExpr();
        expr->setOpcode(expr->opcodeFromString($op.text));
        $val = expr;
    }
    | expr op=('+' | '-') expr {
        auto expr = new minicc::BinaryExpr();
        expr->setOpcode(expr->opcodeFromString($op.text));
        $val = expr;
    }
    | v=('true' | 'false') {
        bool value = $v.text == "true";
        auto expr = new minicc::BoolLiteralExpr(value);
        $val = expr;
    }
    | expr op=('==' | '!=' | '<' | '<=' | '>' | '>=') expr {
        auto expr = new minicc::BinaryExpr();
        expr->setOpcode(expr->opcodeFromString($op.text));
        $val = expr;
    }
    | expr op='&&' expr {
        auto expr = new minicc::BinaryExpr();
        expr->setOpcode(expr->opcodeFromString($op.text));
        $val = expr;
    }
    | expr op='||' expr {
        auto expr = new minicc::BinaryExpr();
        expr->setOpcode(expr->opcodeFromString($op.text));
        $val = expr;
    }
    | '(' expr ')' {
        $val = $expr.val;
    }
    | var {
        $val = new minicc::VarExpr();
        $val->addChild($var.val);
    }
    | funcname '(' arguments ')' {
        auto expr = new minicc::CallExpr();
        if ($arguments.val) {
            for (auto argVal: *$arguments.val) {
                expr->addChild(argVal);
            }
        }
        $val = expr;
    }
    | var '=' expr {
        auto expr = new minicc::AssignmentExpr();
        expr->addChild($var.val);
        expr->addChild($expr.val);
        $val = expr;
    }
    | parametername {
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
:   varname {
        auto id = new minicc::Identifier($varname.text);
        $val->addChild(id);
        id->setParent($val);
    }
    | varname '[' expr ']' {
        auto id = new minicc::Identifier($varname.text);
        $val->addChild(id);
        $expr.val->setParent($val);
        $val->addChild($expr.val);
    }
    ;

arguments returns [std::vector<minicc::Expr*> *val]
:   argumentlist? {
        $val = $argumentlist.val;
    }
    ;
argumentlist returns [std::vector<minicc::Expr*> *val]
@init {
    $val = new std::vector<minicc::Expr*>();
}
:   expr {
        $val->emplace_back($expr.val);
    }
    | args1_node=argumentlist ',' expr {
        $val = $args1_node.val;
        $val->emplace_back($expr.val);
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
CHAR:   '\''.'\'';
WS:     [ \t\r\n]+ -> skip;
COMMENT: '//' (~[\r\n])* -> skip;
