#include <stdio.h>
#include <stdlib.h>
#include "estructuras.h"
#include <stdarg.h> 
#include <string.h>

extern int yylineno;

Simbolo *cabezaTabla = NULL;

// busca un símbolo en la tabla

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
// guarda un nuevo símbolo en la tabla, si no existe previamente
void insertarSimbolo(char *nombre, TipoDato tipo) {
  if (buscarSimbolo(nombre) != NULL) {
    printf("Error: La variable '%s' ya está declarada.\n", nombre);
    exit(1);
  }

  Simbolo *nuevo = malloc(sizeof(Simbolo));
  nuevo->nombre = nombre;
  nuevo->tipo = tipo;
  nuevo->valor = 0;

  nuevo->sig = cabezaTabla;
  cabezaTabla = nuevo;
}

// crear un nuevo nodo para el AST
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

// Evalua el AST y realiza la ejecución semántica (intérprete)
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
        
        case SUMA: {
            int val_izq = evaluarAST(n->izq);
            int val_der = evaluarAST(n->der);
            
            if (n->izq->info->tipo_dato != TIPO_INT || n->der->info->tipo_dato != TIPO_INT){
              printf("Error Semántico [Línea %d]: Suma inválida, ambos operandos deben ser enteros.\n", n->info->linea);
              exit(1);
            }
            n->info->tipo_dato = TIPO_INT;
            return val_izq + val_der;
        }
            
        case MULT: {
            int val_izq = evaluarAST(n->izq);
            int val_der = evaluarAST(n->der);
            
            if (n->izq->info->tipo_dato != TIPO_INT || n->der->info->tipo_dato != TIPO_INT){
              printf("Error Semántico [Línea %d]: Multiplicación inválida, ambos operandos deben ser enteros.\n", n->info->linea);
              exit(1);
            }
            n->info->tipo_dato = TIPO_INT;
            return val_izq * val_der;
        }
            
        case NRO:
            n->info->tipo_dato = TIPO_INT;
            return atoi(n->info->valor);

        case TRUE:
            n->info->tipo_dato = TIPO_BOOL;
            return 1;

        case FALSE:
            n->info->tipo_dato = TIPO_BOOL;
            return 0;

        case AND: {
            int val_izq = evaluarAST(n->izq);
            int val_der = evaluarAST(n->der);
            
            if (n->izq->info->tipo_dato != TIPO_BOOL || n->der->info->tipo_dato != TIPO_BOOL){
              printf("Error Semántico [Línea %d]: Conjunción inválida, ambos operandos deben ser booleanos.\n", n->info->linea);
              exit(1);
            }
            n->info->tipo_dato = TIPO_BOOL;
            return val_izq && val_der;
        }

        case OR: {
            int val_izq = evaluarAST(n->izq);
            int val_der = evaluarAST(n->der);

            if (n->izq->info->tipo_dato != TIPO_BOOL || n->der->info->tipo_dato != TIPO_BOOL){
              printf("Error Semántico [Línea %d]: Disyunción inválida, ambos operandos deben ser booleanos.\n", n->info->linea);
              exit(1);
            }
            n->info->tipo_dato = TIPO_BOOL;
            return val_izq || val_der;
        }

        case NOT: {
            int val = evaluarAST(n->izq);
            
            if (n->izq->info->tipo_dato != TIPO_BOOL){
              printf("Error Semántico [Línea %d]: Negación inválida, el operando debe ser booleano.\n", n->info->linea);
              exit(1);
            }
            n->info->tipo_dato = TIPO_BOOL;
            return !val;
        }

        case ID: {
            Simbolo *s = buscarSimbolo(n->info->valor);
            if (s == NULL) {
                printf("Error Semántico [Línea %d]: Variable '%s' no declarada.\n", n->info->linea, n->info->valor);
                exit(1);
            }
            n->info->tipo_dato = s->tipo;
            return s->valor;
        }

        case ASIG: {
            // n->izq es el ID, n->der es la expresión
            char *nombre_var = n->izq->info->valor;
            Simbolo *s = buscarSimbolo(nombre_var);
            if (s == NULL) {
                printf("Error Semántico [Línea %d]: Asignación a variable '%s' no declarada.\n", n->info->linea, nombre_var);
                exit(1);
            }
            
            int resultado = evaluarAST(n->der);
            
            // chequeo de tipos para variables booleanas
            if (s->tipo != n->der->info->tipo_dato) {
                 printf("Error Semántico [Línea %d]: Conflicto de tipos. No puedes guardar ese valor en la variable '%s'.\n", n->info->linea, nombre_var);
                exit(1);
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

// parte de generación de pseudo assembly
static FILE *emitter_outf = NULL;
static int label_counter = 0;

void emitter_init(FILE *out) {
    emitter_outf = out ? out : stdout;
}

// genera una línea de texto en el archivo de salida
void emit(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    if (!emitter_outf) emitter_outf = stdout;
    vfprintf(emitter_outf, fmt, ap);
    fprintf(emitter_outf, "\n");
    va_end(ap);
}

int new_label(void) {
    label_counter++;
    return label_counter;
}

static void cg_gen_expr(Nodo *n);
static void cg_gen_stmt(Nodo *n);

// genera lineas de pseudo assembly a partir de los nodos del AST
static void cg_gen_expr(Nodo *n) {
    if (!n) return;
    switch (n->info->tipo) {
        case NRO:
            emit("PUSH %s", n->info->valor);
            n->info->tipo_dato = TIPO_INT;
            break;
        case TRUE:
            emit("PUSH 1");
            n->info->tipo_dato = TIPO_BOOL;
            break;
        case FALSE:
            emit("PUSH 0");
            n->info->tipo_dato = TIPO_BOOL;
            break;
        case ID:
            emit("LOAD %s", n->info->valor);
            // tipo lo rellena la tabla de símbolos si corresponde
            break;
        case SUMA:
            cg_gen_expr(n->izq);
            cg_gen_expr(n->der);
            emit("ADD");
            n->info->tipo_dato = TIPO_INT;
            break;
        case MULT:
            cg_gen_expr(n->izq);
            cg_gen_expr(n->der);
            emit("MUL");
            n->info->tipo_dato = TIPO_INT;
            break;
        case AND:
            cg_gen_expr(n->izq);
            cg_gen_expr(n->der);
            emit("AND");
            n->info->tipo_dato = TIPO_BOOL;
            break;
        case OR:
            cg_gen_expr(n->izq);
            cg_gen_expr(n->der);
            emit("OR");
            n->info->tipo_dato = TIPO_BOOL;
            break;
        case NOT:
            cg_gen_expr(n->izq);
            emit("NOT");
            n->info->tipo_dato = TIPO_BOOL;
            break;
        case ASIG:
            // izquierda es ID
            cg_gen_expr(n->der);
            emit("STORE %s", n->izq->info->valor);
            n->info->tipo_dato = n->der->info->tipo_dato;
            break;
        default:
            // si llega algo inesperado, intenta tratarlo como statement
            cg_gen_stmt(n);
            break;
    }
}

// funcion auxiliar que trata los statements y declaraciones
static void cg_gen_stmt(Nodo *n) {
    if (!n) return;
    switch (n->info->tipo) {
        case S:
            // listas de statements: recorrer izq y der
            if (n->izq) cg_gen_stmt(n->izq);
            if (n->der) cg_gen_stmt(n->der);
            break;
        case D:
            // declaraciones: no emitir código (la tabla de símbolos ya debe existir)
            if (n->izq) cg_gen_stmt(n->izq);
            if (n->der) cg_gen_stmt(n->der);
            break;
        case DECL:
            // nada que emitir aquí
            break;
        case RETURN:
            if (n->izq) {
                cg_gen_expr(n->izq);
            } else {
                emit("PUSH 0");
            }
            emit("PRINT"); // para este ejemplo imprimimos el valor retornado
            emit("END");
            break;
        case BLOQUE:
        case PROG:
            if (n->izq) cg_gen_stmt(n->izq);
            if (n->der) cg_gen_stmt(n->der);
            break;
        default:
            // expr-statement (E ';') llega como E en S
            cg_gen_expr(n);
            break;
    }
}

// función principal de generación de assembly
void generar_pseudo(Nodo *root, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) {
        perror("fopen generar_pseudo");
        return;
    }
    emitter_init(f);
    emit("; pseudo-assembly generado");
    cg_gen_stmt(root);
    emit("END");
    fclose(f);
    
}