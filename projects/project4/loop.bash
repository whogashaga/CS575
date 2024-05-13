#!/bin/bash
for t in $(seq 1024 16384 1049600)
do
    g++   proj04.cpp  -DARRAYSIZE=$t  -o proj04  -lm  -fopenmp
    ./proj04
  
done