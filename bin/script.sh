#!/bin/sh

echo "F01" > ../Subresultados/resultadosHistogramaMutacion3.txt; \
for filename in ../data/*.txt; do
  ./QKP $filename >> ../Subresultados/resultadosHistogramaMutacion3.txt
done
