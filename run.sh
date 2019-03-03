#!/bin/bash
clear
gcc -g -c parser.c lexer.c hashTable.c
gcc parser.o lexer.o hashTable.o -o program
rm parser.o
rm lexer.o
rm hashTable.o
./program
