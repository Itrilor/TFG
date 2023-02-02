#!/bin/sh
echo "milestone, 1, 2, 3, 5, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100" > resultadosMilestones.txt; \
CONTADOR=0
for filename in ./data/*.txt; do
  ./bin/QKP $filename >> resultadosMilestones.txt
done
