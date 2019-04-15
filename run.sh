#!/bin/bash
clear
gcc -g -c driver.c parser.c lexer.c hashTable.c AST.c symbolTable.c typeChecker.c semanticAnalyzer.c
gcc driver.o parser.o lexer.o hashTable.o AST.o symbolTable.o typeChecker.o semanticAnalyzer.o -o program
rm driver.o
rm parser.o
rm lexer.o
rm hashTable.o
rm AST.o
rm symbolTable.o
rm typeChecker.o
rm semanticAnalyzer.o
./program semantic/testcase1.txt
