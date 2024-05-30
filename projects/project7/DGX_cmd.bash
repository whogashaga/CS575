#!/bin/bash

ssh submit-c.hpc.engr.oregonstate.edu

module load slurm

squeue  # check on the queues

sinfo   # System Information