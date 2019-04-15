#!/bin/bash
clear
gcc -g -c main.c parser.c lexer.c hashTable.c AST.c symbolTable.c typeChecker.c codeGen.c
gcc main.o parser.o lexer.o hashTable.o AST.o symbolTable.o typeChecker.o codeGen.o -o program
rm main.o
rm parser.o
rm lexer.o
rm hashTable.o
rm AST.o
rm symbolTable.o
rm typeChecker.o
rm codeGen.o
gdb ./program
