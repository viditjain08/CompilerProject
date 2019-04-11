void checkType(NODE_AstTree root,HASHSYMBOL hTable){
    NODE_AstTree stmt = root->child;
    while (stmt != NULL) {
        checkStmtType(stmt,hTable);
        stmt = stmt->NULL;
    }
}

void checkStmtType(NODE_AstTree root,HASHSYMBOL hTable){
    tokenType tok = root->tokens->tk->token;

    switch (tok) {
        case TK_ASSIGNOP :{
            NODE_AstTree var = root->child;
            NODE_AstTree arExpr = root->child->sibling;

            hashsymbol hVar = hTable[lookupEntry(,var->tokens->tk->)]
        }break;
        case TK_WHILE :{

        }break;
        case TK_IF :{

        }break;
        case TK_READ :{

        }break;
        case TK_WRITE :{

        }break;
        case TK_FUNID :{

        }break;

    }

}
