#!/bin/sh

echo "F01" > ../Subresultados/resultadosHistogramaMutacion2.txt; \
for filename in ../data/*.txt; do
  echo $filename
  ./QKP $filename >> ../Subresultados/resultadosHistogramaMutacion2.txt
done
