%{
#include <stdio.h>
%}

nro    [0-9]+
letra     [a-zA-Z]
id     {letra}({letra}|{nro})*

%%
"int"    { return "int"; }
"bool"   { return "bool"; }
"void"   { return "void"; }
"return"   { return "return"; }
"main"   { return "main"; }
"and"   { return "and"; }
"not"   { return "not"; }
"or"   { return "or"; }
"true"   { return "true"; }
"false"   { return "false"; }
";"   { return ';'; }
"+"   { return '+'; }
"("   { return '('; }
")"   { return ')'; }
"}"   { return '}'; }
"{"   { return '{'; }
"="   { return '='; }

{id}       { return yytext; }
{nro}+     { return yytext; }
[ \t\n]+      ; /* Ignorar espacios en blanco y saltos de línea */
.             { printf("Caracter desconocido: %s\n", yytext); }
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
