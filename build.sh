#!/bin/bash

# Define the source and output files
SOURCE_FILES="cache_simulator.c cache_simulator.h"   # List all the source files here
OUTPUT_FILE="build/cache_simulator"           # The name of the output executable

# Compile the program using gcc
echo "Compiling source files..."
gcc $SOURCE_FILES -o $OUTPUT_FILE

# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo "Build successful. Executable is $OUTPUT_FILE"
else
    echo "Build failed."
    exit 1
fi
