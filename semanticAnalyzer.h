#include <stdio.h>
#include <stdlib.h>

void semAnalyze(NODE_AstTree root);
void semStmts(NODE_AstTree root, int funcHashVal);
void checkInitialisations(NODE_AstTree expr, int funcHashVal);
int getHashIndex(NODE_AstTree var, int funcHashVal);
void returnStmtSemantics(NODE_AstTree stmt, int funcHashVal);
void whileLoopSemantic(NODE_AstTree stmt, int funcHashVal);
int checkFunctionSignature(NODE_AstTree par_list, NODE_AstTree par_signature, int funcHashVal);
int assignStmtSemantics(NODE_AstTree root, int funcHashVal);
void ifInit(NODE_AstTree var, int funcHashVal);
