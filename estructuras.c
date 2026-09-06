#include <stdio.h>
#include <stdlib.h>
#include "estructuras.h"

extern int yylineno;

Nodo *crearNodo(TipoNodo tipo, char *valor, Nodo *izq, Nodo *der)
{
    Nodo *n = malloc(sizeof(Nodo));

    n->izq = izq;
    n->der = der;

    n->info = malloc(sizeof(InfoNodo));

    n->info->tipo = tipo;
    n->info->valor = valor;
    n->info->linea = yylineno;
    n->info->tipo_dato = VOID;
    n->info->nombre = NULL;
    n->info->simbolo = NULL;

    return n;
}