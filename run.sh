#!/bin/bash
clear
gcc -g -c main.c parser.c lexer.c hashTable.c ast.c
gcc main.o parser.o lexer.o hashTable.o ast.o -o program
rm main.o
rm parser.o
rm lexer.o
rm hashTable.o
rm ast.o
