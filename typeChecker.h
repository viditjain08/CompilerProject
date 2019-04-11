void checkType(NODE_AstTree root,HASHSYMBOL hTable, FN_STACK st);
void checkStmtType(NODE_AstTree root,HASHSYMBOL hTable, FN_STACK st);
dataType getIdentifierDtype(NODE_AstTree var, HASHSYMBOL hTable, FN_STACK st);
dataType getExpressionDtype(NODE_AstTree root, HASHSYMBOL hTable, FN_STACK st);
dataType matchRecordType(NODE_AstTree child1, NODE_AstTree child2, HASHSYMBOL hTable, FN_STACK st);
