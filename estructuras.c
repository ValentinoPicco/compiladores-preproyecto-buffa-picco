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
    n->info->tipo_dato = TIPO_VOID;
    n->info->nombre = NULL;
    n->info->simbolo = NULL;

    return n;
}

const char* tipoToString(TipoNodo t) {
    switch(t) {
        case PROG: return "PROG";
        case BLOQUE: return "BLOQUE";
        case SUMA: return "SUMA";
        case MULT: return "MULT";
        case NRO: return "NRO";
        case NOT: return "NOT";
        case AND: return "AND";
        case OR: return "OR";
        case TRUE: return "TRUE";
        case FALSE: return "FALSE";
        case ASIG: return "ASIG";
        case RETURN: return "RETURN";
        case INT: return "INT";
        case BOOL: return "BOOL";
        case VOID: return "VOID";
        case MAIN: return "MAIN";
        case ID: return "ID";
        case DECL: return "DECL";
        case D: return "D";
        case S: return "S";
        default: return "DESCONOCIDO";
    }
}