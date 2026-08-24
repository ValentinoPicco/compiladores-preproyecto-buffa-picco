%{
#include <stdio.h>
extern FILE *yyin;
int yylex(void);
void yyerror(const char *s);
%}


%token Int Bool Void
%token Return
%token Main
%token And Not Or
%token True False
%token Id Nro

%%

P:
    TRET Main '(' ')' '{' D S '}'
    ;

E:
    E '+' E
    | E '*' E
    | '(' E ')'
    | Nro
    | Not E
    | E And E
    | E Or E
    | True
    | False
    | Id
    | Id '=' E
    ;

RET:
    Return E 
    | Return
    ;

TRET:
    Int
    | Bool
    | Void
    ;

TVAR: 
    Int
    | Bool
    ;

DEC:
    TVAR Id
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

void yyerror(const char *s) {
  fprintf(stderr, "Error sintáctico: %s\n", s);
}

void main(int argc, char** argv) {
  ++argv, --argc;
  if (argc > 0)
    yyin = fopen(argv[0], "r");
  else
    yyin = stdin;

yyparse();
}

int yywrap(void) {
  return 1;
}
