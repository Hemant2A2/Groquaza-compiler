#!/bin/bash

PROJECT_DIR="/Users/hemantayuj/Desktop/compiler/Groquaza/src"
COMPILER="g++-14"
STD_VERSION="-std=c++20"
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
