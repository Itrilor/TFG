#!/bin/sh

echo "F01" > resultadosAGEU450.txt; \
for filename in ./data/*.txt; do
  ./bin/QKP $filename >> resultadosAGEU450.txt
done
