%{
#include <stdio.h>

%}

%%

%token int bool void
%token return
%token main
%token and not or
%token true false
%token id nro

%%

P:
    TRet main '(' ')' '{' D S '}'
    ;

E:
    E '+' E
    | E '*' E
    | '(' E ')'
    | nro
    | not E
    | E and E
    | E or E
    | true
    | false
    | id
    | id '=' Error
    ;

RET:
    return E 
    | return
    ;

TRet:
    int
    | bool
    | void
    ;

TVar: 
    int
    | bool
    ;

DEC:
    TVar id
    ;

D:
    D DEC ';'
    | 
    ;

S:
    S E ';'
    | S RET ';'
    |
    ;

%%


void main(int argc, char** argv) {

  ++argv, --argc;
  if (argc > 0)
    yyin = fopen(argv[0], "r");
  else
    yyin = stdin;

yylex();

}

int yywrap(void) {
  return 1;
}