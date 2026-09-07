#!/bin/bash

# Compilar proyecto
echo "Compilando el proyecto..."
bison -d bison.y
flex lexico.lex
gcc bison.tab.c lex.yy.c estructuras.c -o prueba -lfl

if [ $? -ne 0 ]; then
    echo "Error al compilar."
    exit 1
fi

echo "Compilación exitosa. Ejecutando tests..."
echo ""

pasados=0
total=0

for file in tests/*.txt; do
    total=$((total+1))
    echo "=========================================================="
    echo "TEST: $file"
    echo "----------------------------------------------------------"
    echo "RESULTADO:"
    ./prueba "$file"
    exit_code=$?
    
    echo "----------------------------------------------------------"
    if [[ "$file" == *"exito"* ]]; then
        if [ $exit_code -eq 0 ]; then
            echo "TEST SUPERADO"
            pasados=$((pasados+1))
        else
            echo "TEST FALLIDO (El compilador arrojó error y se esperaba éxito)"
        fi
    else
        if [ $exit_code -ne 0 ]; then
            echo "TEST SUPERADO (Atrapó el error correctamente)"
            pasados=$((pasados+1))
        else
            echo "TEST FALLIDO (El compilador compiló con éxito algo que estaba mal)"
        fi
    fi
    echo ""
done

echo "=========================================================="
echo "RESULTADO FINAL: $pasados / $total tests pasaron exitosamente."
echo "=========================================================="
