#!/bin/bash
clear
gcc -g -c main.c parser.c lexer.c hashTable.c AST.c symbolTable.c
gcc main.o parser.o lexer.o hashTable.o AST.o symbolTable.o -o program
rm main.o
rm parser.o
rm lexer.o
rm hashTable.o
rm AST.o
rm symbolTable.o
./program
