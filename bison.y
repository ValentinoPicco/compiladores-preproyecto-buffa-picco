%{
#include <stdio.h>
extern FILE *yyin;
int yylex(void);
void yyerror(const char *s);
extern int yylineno; 

typedef struct Nodo {
    char *tipo;
    char *valor;
    struct Nodo *izq;
    struct Nodo *der;
} Nodo;


Nodo *crearNodo(char *tipo, char *valor, Nodo *izq, Nodo *der);
%}

%union {
    Nodo *nodo;
    char *texto;
}

%define parse.error verbose
%type <nodo> P E RET TRET TVAR DEC D S

%right '='
%left Or
%left And
%left '+'
%left '*'
%right Not

%token Int Bool Void
%token Return
%token Main
%token And Not Or
%token True False
%token <texto> Id Nro

%%

P:
    TRET Main '(' ')' '{' D S '}'
    {
        Nodo *bloque = crearNodo("bloque", NULL, $6, $7);
        $$ = crearNodo("prog", NULL, $1, bloque);
    }

    ;

E:
    E '+' E
    {
        $$ = crearNodo("+", NULL, $1, $3);
    }

    | E '*' E
    {
        $$ = crearNodo("*", NULL, $1, $3);
    }

    | '(' E ')'
    {
        $$ = $2;
    }

    | Nro
    {
        $$ = crearNodo("nro", $1, NULL, NULL);
    }

    | Not E
    {
        $$ = crearNodo("not", NULL, $2, NULL);
    }

    | E And E
    {
        $$ = crearNodo("and", NULL, $1, $3);
    }

    | E Or E
    {
        $$ = crearNodo("or", NULL, $1, $3);
    }

    | True
    {
        $$ = crearNodo("true", NULL, NULL, NULL);
    }

    | False
    {
        $$ = crearNodo("false", NULL, NULL, NULL);
    }

    | Id
    {
        $$ = crearNodo("id", $1, NULL, NULL);
    }

    | Id '=' E
    {
        Nodo *id = crearNodo("id", $1, NULL, NULL);
        $$ = crearNodo("=", NULL, id, $3);
    }

    ;

RET:
    Return E 
    {
        $$ = crearNodo("return", NULL, $2, NULL);
    }

    | Return
    {
        $$ = crearNodo("return", NULL, NULL, NULL);
    }

    ;

TRET:
    Int
    {
        $$ = crearNodo("int", NULL, NULL, NULL);
    }

    | Bool
    {
        $$ = crearNodo("bool", NULL, NULL, NULL);
    }

    | Void
    { 
        $$ = crearNodo("void", NULL, NULL, NULL);
    }

    ;

TVAR: 
    Int
    {
        $$ = crearNodo("int", NULL, NULL, NULL);
    }

    | Bool
    {
        $$ = crearNodo("bool", NULL, NULL, NULL);
    }

    ;

DEC:
    TVAR Id
    {   
        Nodo *id = crearNodo("id", $2, NULL, NULL);
        $$ = crearNodo("decl", NULL, $1, id);
    }

    ;

D:
    D DEC ';'
    {
        $$ = crearNodo("D", NULL, $1, $2);
    }

    | 
    {
        $$ = NULL;
    }

    ;

S:
    S E ';'
    {
        $$ = crearNodo("S", NULL, $1, $2);
    }

    | S RET ';'
    {
        $$ = crearNodo("S", NULL, $1, $2);
    }
    
    |
    {
        $$ = NULL;
    }
    ;

%%

void yyerror(const char *s) {
  fprintf(stderr, "Error en la línea %d: %s\n", yylineno, s); 
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


