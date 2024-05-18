#!/bin/csh
for t in  8 32 64 128 256
do
    /usr/local/apps/cuda/cuda-10.1/bin/nvcc -DTHREADS_PER_BLOCK=$t -o arrayMul arrayMul.cu # make file
    ./arrayMul  # Run the runnable
done