#!/bin/bash
# Define variables for number of nodes
NUM_NODES=8
TOTAL_MPI_TASKS=8
MPI_PROCESSES_PER_NODE=4

#SBATCH -J AutoCorr
#SBATCH -A cs475-575
#SBATCH -p classmpifinal
#SBATCH -N 8 # number of nodes
#SBATCH -n 8 # number of tasks
#SBATCH -o mpiproject.out
#SBATCH -e mpiproject.err
#SBATCH --mail-type=END,FAIL
#SBATCH --mail-user=chenyenc@oregonstate.edu
module load openmpi
for np in 1 2 4 8 12 16 20 24
do
    mpic++ proj07.cpp -o proj07 -lm
    mpiexec -mca btl self,tcp -np $np ./proj07
done