compileAll	:	AST.c ast.h astDef.h driver.c hashTable.c hashTable.h lexer.c lexer.h lexerDef.h parser.c parser.h parserDef.h semanticAnalyzer.h semanticAnalyzerDef.h semanticAnalyzer.c symbolTable.c symbolTable.h symbolTabledef.h typeChecker.c typeChecker.h codeGen.h codeGenDef.h codeGen.c
	gcc -g -c driver.c parser.c lexer.c hashTable.c AST.c symbolTable.c typeChecker.c semanticAnalyzer.c codeGen.c
	gcc driver.o parser.o lexer.o hashTable.o AST.o symbolTable.o typeChecker.o semanticAnalyzer.o codeGen.o -o program
	rm driver.o
	rm parser.o
	rm lexer.o
	rm hashTable.o
	rm AST.o
	rm symbolTable.o
	rm typeChecker.o
	rm semanticAnalyzer.o
	rm codeGen.o
