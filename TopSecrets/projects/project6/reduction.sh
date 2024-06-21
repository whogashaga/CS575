#!/bin/bash
#SBATCH  --constraint=v100

NMB=(1 10 100 1000 2000 4000 8000)
local=(32 64 128 256)

if [ -f "./mult_red.csv" ]; then
    rm mult_red.csv
fi
echo "NMB,Global Dataset Size,Local Work Size,Work Goups,Performance" |& tee -a mult_red.csv
for n in ${NMB[@]};
do
    for l in ${local[@]};
    do
        g++ -o multRed multRed.cpp /usr/local/apps/cuda/10.1/lib64/libOpenCL.so.1.1 -DNMB=$n -DLOCAL_SIZE=$l -lm -fopenmp
        ./multRed |& tee -a mult_red.csv
    done
done
