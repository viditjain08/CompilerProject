#!/bin/bash
clear
gcc -g -c driver.c parser.c lexer.c hashTable.c
gcc driver.o parser.o lexer.o hashTable.o -o program
rm driver.o
rm parser.o
rm lexer.o
rm hashTable.o
./program testcases/testcase4.txt
