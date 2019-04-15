// void checkType(NODE_AstTree root);
// void checkStmtsType(NODE_AstTree root, int funcHashVal);
// void checkStmtType(NODE_AstTree root, int funcHashVal);
#include <stdlib.h>
#include <stdio.h>
#include "symbolTabledef.h"

extern int errno;

char* getIdentifierDtype(NODE_AstTree var, int funcHashVal);
char* getExpressionDtype(NODE_AstTree root, int funcHashVal);
// dataType matchRecordType(NODE_AstTree child1, NODE_AstTree child2, int funcHashVal);
int getHashIndex(NODE_AstTree var, int funcHashVal);
