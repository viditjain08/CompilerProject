/*
    Group 1:
    1. Anupam Aggarwal     2016A7PS0033P
    2. Piyush Garg         2016A7PS0035P
    3. Rijul Katiyar       2016A7PS0063P
    4. Vidit Jain          2016A7PS0064P

*/
#include <stdio.h>
#include <stdlib.h>

void semAnalyze(NODE_AstTree root);
void semStmts(NODE_AstTree root, int funcHashVal);
void checkInitialisations(NODE_AstTree expr, int funcHashVal);
void returnStmtSemantics(NODE_AstTree stmt, int funcHashVal);
void whileLoopSemantic(NODE_AstTree stmt, int funcHashVal);
int checkFunctionSignature(NODE_AstTree par_list, NODE_AstTree par_signature, int funcHashVal);
int assignStmtSemantics(NODE_AstTree root, int funcHashVal);
void ifInit(NODE_AstTree var, int funcHashVal);
void initInputParameters(NODE_AstTree root) ;
int checkFunctionInvoke(NODE_AstTree stmt, int funcHashVal);
void printErrors(int n);
void addError(int lineNo);

int checkUpdate(NODE_AstTree var, NODE_AstTree stmts);
VARLIST extractAllVar(NODE_AstTree expr, VARLIST vars);
VARLIST addVar(NODE_AstTree var, VARLIST ls);
