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

echo "Compilacion exitosa. Ejecutando tests..."
echo ""

pasados=0
total=0

for file in tests/*.txt; do
    total=$((total+1))
    echo "=========================================================="
    echo "TEST: $file"
    echo "----------------------------------------------------------"
    
    rm -f pseudoassembly.txt
    
    ./prueba "$file"
    exit_code=$?
    
    if [[ "$file" == *"exito"* || "$file" == *"cg_"* ]]; then
        if [ $exit_code -eq 0 ]; then
            echo "----------------------------------------------------------"
            echo "TEST PASADO"
            pasados=$((pasados+1))
            echo "PseudoAssembly Generado (pseudoassembly.txt):"
            sed 's/^/   | /' pseudoassembly.txt
        else
            echo "----------------------------------------------------------"
            echo "TEST FALLIDO (El compilador arrojo error y se esperaba exito)"
        fi
    else
        if [ $exit_code -ne 0 ]; then
            echo "----------------------------------------------------------"
            echo "TEST PASADO (Atrapo el error correctamente)"
            pasados=$((pasados+1))
        else
            echo "----------------------------------------------------------"
            echo "TEST FALLIDO (El compilador compilo con exito algo que estaba mal)"
        fi
    fi
    echo ""
done

echo "=========================================================="
echo "RESULTADO FINAL: $pasados / $total tests pasaron exitosamente."
echo "=========================================================="
