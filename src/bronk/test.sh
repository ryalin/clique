#!/bin/bash

# Set the common flags
COMMON_FLAGS="-t 10 -d 0.1 -s 100"

# Specify the range of values for the -d flag
START_D=2
END_D=8
STEP_D=1

# Create an array of values
d_values=($(seq $START_D $STEP_D $END_D))

# Loop through the values of -d
for current_d in "${d_values[@]}"; do
    # Construct the full command with the current -d value
    COMMAND="make run FLAGS=\"$COMMON_FLAGS\""

    # Print the command (optional)
    echo "Running command: $COMMAND"
    export OMP_NUM_THREADS="$current_d"
    # Execute the command
    eval $COMMAND

    # Add any additional logic or actions you want to perform after each iteration
done