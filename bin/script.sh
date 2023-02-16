#!/bin/sh

echo "F01" > ../Subresultados/resultadosHistogramaMutacion.txt; \
for filename in ../data/*.txt; do
  ./QKP $filename >> ../Subresultados/resultadosHistogramaMutacion.txt
done
