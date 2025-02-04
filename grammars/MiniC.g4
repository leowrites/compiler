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
@init {
    // std::cerr << "[DEBUG] Entering preamble" << std::endl;
    $headerIncluded = false;
}
@after {
    // std::cerr << "[DEBUG] Leaving preamble" << std::endl;
}
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
    }
    |   rettype funcname '(' parameters ')' ';'
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
scope
/*You may need init in Assignment 3*/
@init {}
:  '{' (vardecl)* (stmt )* '}'
    ;
stmt
:   expr ';' 
    |   'if' '(' expr ')' stmt 
    |   'if' '(' expr ')' s1=stmt 'else' s2=stmt 
    |   'for' '(' e1=expropt ';' e2=expropt ';' e3=expropt ')' stmt
    |   'while' '(' expr ')' stmt 
    |   'break' ';'
    |   'return' ';' 
    |   'return' expr ';' 
    |   scope 
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
@init {
    // std::cerr << "[DEBUG] Entering vartype" << std::endl;
}
@init {
    // std::cerr << "[DEBUG] Leaving vartype" << std::endl;
}
:   t=('int' | 'bool' | 'char') {
        $val = new minicc::TypeReference(
            $t.text == "int"  ? minicc::Type::PrimitiveTypeEnum::Int  :
            $t.text == "bool" ? minicc::Type::PrimitiveTypeEnum::Bool :
                          minicc::Type::PrimitiveTypeEnum::Char
        );
    }
;
rettype returns [minicc::TypeReference *val]
:    'void' {
        $val = new minicc::TypeReference(minicc::Type::PrimitiveTypeEnum::Void);
    }
    |   vartype {
        $val = $vartype.val;
    }
    ;
parameters
:     /* eps */
    |   parameterlist
    ;
parameterlist
:     parameterentry
    |   p1=parameterlist ',' parameterentry
    ;
parameterentry
:     vartype parametername
    ;
expropt
:    expr
    |   /* eps */
    ;
expr
:   INT
    |   CHAR
    |   '-' expr
    |   e1=expr op=('*'|'/') e2=expr
    |   e1=expr op=('+'|'-') e2=expr
    |   e1=expr op=('==' | '!=' | '<' | '<=' | '>' | '>=') e2=expr
    |   '!' expr
    |   e1=expr '&&' e2=expr
    |   e1=expr '||' e2=expr
    |   var '=' expr
    |   'true'
    |   'false'
    |   '(' expr ')'
    |   var
    |   funcname '(' arguments ')'
    |   parametername
    ;
var
:    varname
    |   varname '[' expr ']'
    ;
arguments
:   /* epsilon */
    |   argumentlist
    ;
argumentlist
:   expr
    |   a1=argumentlist ',' expr
    ;
varname returns [minicc::Identifier *val]
@init {
    // std::cerr << "[DEBUG] Entering varname" << std::endl;
}
@after {
    // std::cerr << "[DEBUG] Leaving varname" << std::endl;
}
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
