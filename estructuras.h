#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

typedef enum TipoNodo {
    PROG, BLOQUE, SUMA, MULT, NRO, NOT, AND, OR, TRUE, FALSE,
    ASIG, RETURN, INT, BOOL, VOID, MAIN, ID, DECL, D, S
} TipoNodo;

typedef enum TipoDato {
    INT, BOOL, VOID
} TipoDato;

typedef struct InfoNodo {
    TipoNodo tipo;
    char *valor;
    int linea;
    TipoDato tipo_dato;
    char *nombre;
    void *simbolo;
} InfoNodo;

typedef struct Nodo {
    InfoNodo *info;
    struct Nodo *izq;
    struct Nodo *der;
} Nodo;

Nodo *crearNodo(TipoNodo tipo, char *valor, Nodo *izq, Nodo *der);

#endif