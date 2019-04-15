/*
    Group 1:
    1. Anupam Aggarwal     2016A7PS0033P
    2. Piyush Garg         2016A7PS0035P
    3. Rijul Katiyar       2016A7PS0063P
    4. Vidit Jain          2016A7PS0064P

*/

#include <stdlib.h>
#include <stdio.h>
#include "symbolTabledef.h"

extern int errno;

char* getIdentifierDtype(NODE_AstTree var, int funcHashVal);
char* getExpressionDtype(NODE_AstTree root, int funcHashVal);
// dataType matchRecordType(NODE_AstTree child1, NODE_AstTree child2, int funcHashVal);
int getHashIndex(NODE_AstTree var, int funcHashVal);
