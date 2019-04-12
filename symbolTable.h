#include <stdio.h>
#include <stdlib.h>

SYMBOLTABLE symbolTableinit(char* name);
HASHSYMBOL populateSymbolTable(NODE_AstTree ast);
int getFunction(char* function);
int lookupEntry(char *function, char *id, char *fieldid) ;
