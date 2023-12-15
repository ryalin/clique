#!/bin/bash

COMMON_FLAGS="-t 6 -d 0.05 -s 200"

START_D=2
END_D=8
STEP_D=1

d_values=($(seq $START_D $STEP_D $END_D))

for current_d in "${d_values[@]}"; do
    COMMAND="make run FLAGS=\"$COMMON_FLAGS\""

    echo "Running command: $COMMAND"
    export OMP_NUM_THREADS="$current_d"
    eval $COMMAND
done

