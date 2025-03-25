#!/bin/bash

if [ "$#" -ge 1 ]; then
    FILE_ARG="$1"
else
    FILE_ARG=""
fi

OS_NAME=$(uname -s)
if [[ "$OS_NAME" == "Darwin" ]]; then
    PROJECT_DIR="/Users/hemantayuj/Desktop/compiler/Groquaza/src"
    STD_VERSION="-std=c++20"
elif [[ "$OS_NAME" == "Linux" ]]; then
    PROJECT_DIR="/home/h3m/Desktop/Projects/Groquaza-compiler/src"
    STD_VERSION="-std=c++2a"
else 
    echo "Unsupported Os"
    exit 1
fi

COMPILER=$(which g++-14 || which g++ || echo "nope")
if [[ "$COMPILER" == "nope" ]]; then
    echo "g++ not found"
    exit 1
fi

SRC_FILES="main.cpp lexer/Lexer.cpp lexer/reader/Reader.cpp lexer/reader/CodeLoc.cpp parser/Parser.cpp ast/PrintAST.cpp code-generator/AsmEmitter.cpp code-generator/CodeGenerator.cpp runtime/runtime.c symbol-table/SymbolTable.cpp"
OUTPUT="main"

cd "$PROJECT_DIR" || exit
$COMPILER $STD_VERSION $SRC_FILES -o $OUTPUT

ASM_OUTPUT="output.s"

if [ $? -eq 0 ]; then
    if [ -n "$FILE_ARG" ]; then
         ./$OUTPUT "$FILE_ARG" "$ASM_OUTPUT"
    else
         echo "Running program with default file."
         ./$OUTPUT
    fi
else
    echo "Compilation failed."
fi
