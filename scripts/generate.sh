#!/bin/bash

ARCH=$(uname -m)
if [[ "$ARCH" != "aarch64" ]]; then
    echo "Only works for Aarch64 assembly"
    exit 1
fi

PROJECT_DIR="/home/h3m/Desktop/Projects/Groquaza-compiler/src"
TEST_FILE="vector"

cd "$PROJECT_DIR" || { echo "Project directory not found"; exit 1; }

as -o output_"$TEST_FILE".o output_"$TEST_FILE".s || { echo "Assembly failed"; exit 1; }
gcc -c runtime/runtime.c -o runtime.o -std=c11 || { echo "Runtime compilation failed"; exit 1; }
gcc -o output_"$TEST_FILE" output_"$TEST_FILE".o runtime.o -nostartfiles -lc || { echo "Linking failed"; exit 1; }

./output_"$TEST_FILE"
