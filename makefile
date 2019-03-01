compileAll	:	hashTable.o		driver.o	lexer.o
	gcc hashTable.o driver.o lexer.o -o program
	rm hashTable.o
	rm driver.o
	rm lexer.o

lexer.o		:	lexer.c		lexerDef.h	lexer.h		hashTable.h
	gcc -c -g lexer.c

hashTable.o	:	hashTable.c		hashTable.h
	gcc -c -g hashTable.c

driver.o	:	lexerDef.h		driver.c
	gcc -c -g driver.c
