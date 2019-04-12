void checkType(NODE_AstTree root);
void checkStmtsType(NODE_AstTree root, int funcHashVal);
void checkStmtType(NODE_AstTree root, int funcHashVal);
dataType getIdentifierDtype(NODE_AstTree var, int funcHashVal);
dataType getExpressionDtype(NODE_AstTree root, int funcHashVal);
dataType matchRecordType(NODE_AstTree child1, NODE_AstTree child2, int funcHashVal);
void checkFunctionInvoke(NODE_AstTree stmt, int funcHashVal);
