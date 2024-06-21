#!/bin/bash
#SBATCH  --constraint=v100

file=(\"mult.cl\" \"multAdd.cl\")
NMB=(1 10 100 1000 2000 4000 8000)
local=(8 16 32 64 128 256 512)

if [ -f "./mult.csv" ]; then
    rm mult.csv
fi
for f in ${file[@]};
do
    echo "$f,Global Dataset Size,Local Work Size,Work Goups,Performance" |& tee -a mult.csv
    for n in ${NMB[@]};
    do
        for l in ${local[@]};
        do
            g++ -o mult mult.cpp /usr/local/apps/cuda/10.1/lib64/libOpenCL.so.1.1 -DFILE_NAME=$f -DNMB=$n -DLOCAL_SIZE=$l -lm -fopenmp
            ./mult |& tee -a mult.csv
        done
    done
done
