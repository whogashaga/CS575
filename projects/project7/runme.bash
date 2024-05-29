#!/bin/bash
#SBATCH -J AutoCorr
#SBATCH -A cs475-575
#SBATCH -p classmpitest
#SBATCH -N 8
#SBATCH --ntasks-per-node=1
#SBATCH -o proj7.out
#SBATCH -e proj7.err
for b in 1 2 4 6 8
do
  module load openmpi
  mpic++ proj07.cpp -o proj07 -lm
  mpiexec --mca btl_base_warn_component_unused 0 -np $b ./proj07
done