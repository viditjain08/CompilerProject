#include <stdio.h>
#include <stdlib.h>
NODE_AstTree applyRule(TREE_NODE root, NODE_AstTree children);
NODE_AstTree buildAST(TREE_NODE root);
int countNodesAST(NODE_AstTree root);
int countNodesParseTree(TREE_NODE root);

void traverseAST(NODE_AstTree root);
void traverseFuncAST(NODE_AstTree root);
void printTypeDefs(NODE_AstTree root);
void printDef(NODE_AstTree root);
void printParams(NODE_AstTree root);
void printRetStmt(NODE_AstTree root);
void traverseMainFuncAST(NODE_AstTree main);
void printStmts(NODE_AstTree);
void printExpr(NODE_AstTree root);
