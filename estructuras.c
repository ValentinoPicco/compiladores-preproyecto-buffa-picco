#include <stdio.h>
#include <stdlib.h>
#include "estructuras.h"

#include <string.h>

extern int yylineno;

Simbolo *cabezaTabla = NULL;

Simbolo* buscarSimbolo(char *nombre) {
  Simbolo *actual = cabezaTabla;

  while (actual != NULL) {
    if (strcmp(actual->nombre, nombre) == 0){
      return actual;
    }
    actual = actual->sig;
  }

  return NULL;
}

void insertarSimbolo(char *nombre, TipoDato tipo) {
  if (buscarSimbolo(nombre) != NULL) {
    printf("Error: La variable '%s' ya está declarada.\n", nombre);
    return;
  }

  Simbolo *nuevo = malloc(sizeof(Simbolo));
  nuevo->nombre = nombre;
  nuevo->tipo = tipo;
  nuevo->valor = 0;

  nuevo->sig = cabezaTabla;
  cabezaTabla = nuevo;
}

Nodo *crearNodo(TipoNodo tipo, char *valor, Nodo *izq, Nodo *der)
{
    Nodo *n = malloc(sizeof(Nodo));

    n->izq = izq;
    n->der = der;

    n->info = malloc(sizeof(InfoNodo));

    n->info->tipo = tipo;
    n->info->valor = valor;
    n->info->linea = yylineno;
    n->info->tipo_dato = TIPO_NONE;
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

int evaluarAST(Nodo *n) {
    if (n == NULL) return 0;

    switch (n->info->tipo) {
        case PROG:
        case BLOQUE:
        case D:
        case S:
            evaluarAST(n->izq);
            evaluarAST(n->der);
            return 0;
            
        case DECL: {
            TipoNodo tipo_ast = n->izq->info->tipo;
            char *nombre_var = n->der->info->valor;
            TipoDato tipo_final = (tipo_ast == INT) ? TIPO_INT : TIPO_BOOL;
            insertarSimbolo(nombre_var, tipo_final);
            return 0;
        }
        
        case SUMA:
            return evaluarAST(n->izq) + evaluarAST(n->der);
            
        case MULT:
            return evaluarAST(n->izq) * evaluarAST(n->der);
            
        case NRO: 
            return atoi(n->info->valor);

        case TRUE:
            return 1;

        case FALSE:
            return 0;

        case AND:
            return evaluarAST(n->izq) && evaluarAST(n->der);

        case OR:
            return evaluarAST(n->izq) || evaluarAST(n->der);

        case NOT:
            return !evaluarAST(n->izq);

        case ID: {
            Simbolo *s = buscarSimbolo(n->info->valor);
            if (s == NULL) {
                printf("Error Semántico [Línea %d]: Variable '%s' no declarada.\n", n->info->linea, n->info->valor);
                return 0;
            }
            return s->valor;
        }

        case ASIG: {
            // n->izq es el ID, n->der es la expresión
            char *nombre_var = n->izq->info->valor;
            Simbolo *s = buscarSimbolo(nombre_var);
            if (s == NULL) {
                printf("Error Semántico [Línea %d]: Asignación a variable '%s' no declarada.\n", n->info->linea, nombre_var);
                return 0;
            }
            
            int resultado = evaluarAST(n->der);
            
            // Opcional: chequeo de tipos para variables booleanas
            if (s->tipo == TIPO_BOOL && (resultado != 0 && resultado != 1)) {
                 printf("Advertencia [Línea %d]: Asignando un valor que no es boolean (1 o 0) a la variable '%s'.\n", n->info->linea, nombre_var);
            }
            
            s->valor = resultado;
            return resultado;
        }

        case RETURN: {
            int ret = 0;
            if (n->izq != NULL) {
                ret = evaluarAST(n->izq);
            }
            printf("\n>> Ejecución finalizada. RETURN arrojó: %d\n", ret);
            return ret;
        }

        default:
            return 0;
    }
}
