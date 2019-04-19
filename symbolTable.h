/*
    Group 1:
    1. Anupam Aggarwal     2016A7PS0033P
    2. Piyush Garg         2016A7PS0035P
    3. Rijul Katiyar       2016A7PS0063P
    4. Vidit Jain          2016A7PS0064P

*/
#include <stdio.h>
#include <stdlib.h>

SYMBOLTABLE symbolTableinit(char* name);
HASHSYMBOL populateSymbolTable(NODE_AstTree ast);
int getFunction(char* function);
int lookupEntry(char *function, char *id, char *fieldid) ;
FN_STACK stack_init(char* var_name, char* fieldname);
FN_STACK push(FN_STACK f, char* var_name, char* fieldname);
FN_STACK pop(FN_STACK f);
void printSymbolTable(SYMBOLTABLE s);
