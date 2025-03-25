#!/bin/bash

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

echo "Compiling..."
$COMPILER $STD_VERSION $SRC_FILES -o $OUTPUT

if [ $? -eq 0 ]; then
    echo "Compilation successful. Running program..."
    ./$OUTPUT
else
    echo "Compilation failed."
fi
