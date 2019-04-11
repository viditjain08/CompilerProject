#include <stdio.h>
#include <stdlib.h>

SYMBOLTABLE symbolTableinit(char* name);
HASHSYMBOL semanticAnalyzer(NODE_AstTree ast, char** errors);
