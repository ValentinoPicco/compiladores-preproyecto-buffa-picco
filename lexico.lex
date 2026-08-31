%{
#include <stdio.h>
#include "bison.tab.h"
%}

%option yylineno

nro    [0-9]+
letra     [a-zA-Z]
id     {letra}({letra}|{nro})*

%%
"int"    { return Int; }
"bool"   { return Bool; }
"void"   { return Void; }
"return"   { return Return; }
"main"   { return Main; }
"and"   { return And; }
"not"   { return Not; }
"or"   { return Or; }
"true"   { return True; }
"false"   { return False; }
";"   { return ';'; }
"+"   { return '+'; }
"("   { return '('; }
")"   { return ')'; }
"}"   { return '}'; }
"{"   { return '{'; }
"="   { return '='; }

{id}       { return Id; }
{nro}     { return Nro; }
[ \t\n]+      ; /* Ignorar espacios en blanco y saltos de linea */
.             ;
%%
