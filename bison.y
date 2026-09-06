%{
#include <stdio.h>
#include <stdlib.h>
#include "estructuras.h"
extern FILE *yyin;
int yylex(void);
void yyerror(const char *s);
extern int yylineno; 

Nodo *raiz = NULL;

%}

%union {
    struct Nodo *nodo;
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
        Nodo *bloque = crearNodo(BLOQUE, NULL, $6, $7);
        bloque->info->linea = $1->info->linea; // Que el bloque herede la misma línea
        $$ = crearNodo(PROG, NULL, $1, bloque);
        $$->info->linea = $1->info->linea;
        raiz = $$;
    }

    ;

E:
    E '+' E
    {
        $$ = crearNodo(SUMA, NULL, $1, $3);
    }

    | E '*' E
    {
        $$ = crearNodo(MULT, NULL, $1, $3);
    }

    | '(' E ')'
    {
        $$ = $2;
    }

    | Nro
    {
        $$ = crearNodo(NRO, $1, NULL, NULL);
    }

    | Not E
    {
        $$ = crearNodo(NOT, NULL, $2, NULL);
    }

    | E And E
    {
        $$ = crearNodo(AND, NULL, $1, $3);
    }

    | E Or E
    {
        $$ = crearNodo(OR, NULL, $1, $3);
    }

    | True
    {
        $$ = crearNodo(TRUE, NULL, NULL, NULL);
    }

    | False
    {
        $$ = crearNodo(FALSE, NULL, NULL, NULL);
    }

    | Id
    {
        $$ = crearNodo(ID, $1, NULL, NULL);
    }

    | Id '=' E
    {
        Nodo *id = crearNodo(ID, $1, NULL, NULL);
        $$ = crearNodo(ASIG, NULL, id, $3);
    }

    ;

RET:
    Return E 
    {
        $$ = crearNodo(RETURN, NULL, $2, NULL);
    }

    | Return
    {
        $$ = crearNodo(RETURN, NULL, NULL, NULL);
    }

    ;

TRET:
    Int
    {
        $$ = crearNodo(INT, NULL, NULL, NULL);
    }

    | Bool
    {
        $$ = crearNodo(BOOL, NULL, NULL, NULL);
    }

    | Void
    { 
        $$ = crearNodo(VOID, NULL, NULL, NULL);
    }

    ;

TVAR: 
    Int
    {
        $$ = crearNodo(INT, NULL, NULL, NULL);
    }

    | Bool
    {
        $$ = crearNodo(BOOL, NULL, NULL, NULL);
    }

    ;

DEC:
    TVAR Id
    {   
        Nodo *id = crearNodo(ID, $2, NULL, NULL);
        $$ = crearNodo(DECL, NULL, $1, id);
    }

    ;

D:
    D DEC ';'
    {
        $$ = crearNodo(D, NULL, $1, $2);
    }

    | 
    {
        $$ = NULL;
    }

    ;

S:
    S E ';'
    {
        $$ = crearNodo(S, NULL, $1, $2);
    }

    | S RET ';'
    {
        $$ = crearNodo(S, NULL, $1, $2);
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

void imprimirAST(Nodo *n, int nivel) {
    if (n == NULL) return;

    for (int i = 0; i < nivel; i++) {
        printf("  ");
    }

    if (n->info->valor != NULL) {
        printf("- %s (%s) [Línea %d]\n", tipoToString(n->info->tipo), n->info->valor, n->info->linea);
    } else {
        printf("- %s [Línea %d]\n", tipoToString(n->info->tipo), n->info->linea);
    }

    imprimirAST(n->izq, nivel + 1);
    imprimirAST(n->der, nivel + 1);
}

void main(int argc, char** argv) {
  ++argv, --argc;
  if (argc > 0)
    yyin = fopen(argv[0], "r");
  else
    yyin = stdin;

  yyparse();
  
  printf("\n--- Árbol Sintáctico Abstracto (AST) ---\n");
  imprimirAST(raiz, 0);
}

int yywrap(void) {
  return 1;
}

