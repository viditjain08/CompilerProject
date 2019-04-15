#include <stdio.h>
#include <stdlib.h>

SYMBOLTABLE symbolTableinit(char* name);
HASHSYMBOL populateSymbolTable(NODE_AstTree ast);
int getFunction(char* function);
int lookupEntry(char *function, char *id, char *fieldid) ;
FN_STACK stack_init(char* var_name, char* fieldname);
FN_STACK push(FN_STACK f, char* var_name, char* fieldname);
FN_STACK pop(FN_STACK f);
