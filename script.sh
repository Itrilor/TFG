#!/bin/sh

echo " ,Random, ,Greedy, ,AGEU, " > resultados.txt; \
echo "caso,Valor,Tiempo(s),Valor,Tiempo(ms),Valor,Tiempo(s)" >> resultados.txt;
for filename in ./data/*.txt; do
  ./bin/QKP $filename >> resultados.txt
done
