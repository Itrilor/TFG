#!/bin/sh
echo "milestone, 1, 2, 3, 5, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100" > ../Subresultados/50tries/GACEP3103_4.txt; \
for filename in ../data/*.txt; do
  echo $filename
  ./QKP $filename >> ../Subresultados/50tries/GACEP3103_4.txt
done
