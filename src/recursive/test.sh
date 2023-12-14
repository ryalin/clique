#!/bin/bash

# Set the common flags
COMMON_FLAGS="-t 3 -s 100"

# Specify the range of values for the -d flag
START_D=0.06
END_D=0.3
STEP_D=0.04

# Create an array of values
d_values=($(seq $START_D $STEP_D $END_D))

# Loop through the values of -d
for current_d in "${d_values[@]}"; do
    # Construct the full command with the current -d value
    COMMAND="make run FLAGS=\"-d $current_d $COMMON_FLAGS\""

    # Print the command (optional)
    echo "Running command: $COMMAND"

    # Execute the command
    eval $COMMAND

    # Add any additional logic or actions you want to perform after each iteration
done