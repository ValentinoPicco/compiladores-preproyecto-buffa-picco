INTEGRANTES: Valentino Picco y Santiago Buffa

INSTRUCCIONES PARA LA EJECUCION:

bison -d bison.y && gcc bison.tab.c lex.yy.c estructuras.c -o prueba -lfl
compila todo y genera el archivo "prueba" como salida (en teoría no debería ser necesario)

./run_tests.sh corre todos los tests de la suite
./prueba "nombre del archivo".txt crea el árbol, corre el intérprete y genera pseudoassembly para 
un archivo de texto en el lenguajehttps://github.com/ValentinoPicco/compiladores-preproyecto-buffa-picco