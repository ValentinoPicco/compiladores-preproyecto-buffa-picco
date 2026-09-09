#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

typedef enum TipoNodo {
    PROG, BLOQUE, SUMA, MULT, NRO, NOT, AND, OR, TRUE, FALSE,
    ASIG, RETURN, INT, BOOL, VOID, MAIN, ID, DECL, D, S
} TipoNodo;

typedef enum TipoDato {
    TIPO_INT, TIPO_BOOL, TIPO_VOID, TIPO_NONE
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
const char* tipoToString(TipoNodo t);

typedef struct Simbolo {
  char *nombre;
  TipoDato tipo;
  int valor;
  struct Simbolo *sig;
} Simbolo;

int evaluarAST(Nodo *n);



void emitter_init(FILE *out);
void emit(const char *fmt, ...);
int new_label(void);

void generar_pseudo(Nodo *root, const char *filename);
#endif

