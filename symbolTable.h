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
