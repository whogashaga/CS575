#!/bin/bash

# number of threads:
for t in 1 4
do
    echo "- - - - - - NUMT = $t - - - - - -"
    g++ proj0.cpp -lm -fopenmp -o proj0
    if [ 1 -eq $t ];
    then
        pOneThread=$(./proj0 1)
    else
        pFourThread=$(./proj0 4)
    fi
done
echo "------------------------------------------------------------------------"
# Speedup = (Peak performance for 4 threads) / (Peak performance for 1 thread)
speedup=$(echo "scale=2; $pFourThread / $pOneThread" | bc)
echo "Speedup = (Peak performance for 4 threads) / (Peak performance for 1 thread) = $speedup"

g++ parallel_fraction.cpp -o parallel_fraction
./parallel_fraction $speedup