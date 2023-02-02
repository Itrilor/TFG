#!/bin/sh

echo "F01" > resultadosAGEUEv.txt; \
for filename in ./data/*.txt; do
  ./bin/QKP $filename >> resultadosAGEUEv.txt
done
