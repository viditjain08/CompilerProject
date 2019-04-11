#include "typeChecker.h"

void checkType(NODE_AstTree root,HASHSYMBOL hTable, FN_STACK st){
    NODE_AstTree stmt = root->child;
    while (stmt != NULL) {
        checkStmtType(stmt,hTable,st);
        stmt = stmt->NULL;
    }
}

void checkStmtType(NODE_AstTree root,HASHSYMBOL hTable, FN_STACK st){
    tokenType tok = root->tokens->tk->token;

    switch (tok) {
        case TK_ASSIGNOP :{
            NODE_AstTree var = root->child;
            NODE_AstTree arExpr = root->child->sibling;

            dataType dtVar = getIdentifierDtype(var,hTable,st);
            dataType dtArExpr = getExpressionDtype(arExpr,hTable,st);
            if(dtVar == dtArExpr){
                if(dtVar == RECORD){// we have to matchRecordType
                    dataType dt = matchRecordType(var,arExpr,hTable,st);
                }else{
                    // there is no type error, HURRAY!
                }
            }else{
                // stmt for error
            }
        }break;
        case TK_WHILE :{
            NODE_AstTree boolExpr = root->child;
            NODE_AstTree stmts = root->child->sibling;
            if(getExpressionDtype(boolExpr)==BOOL){
                checkStmtType(stmts);
            }else{
                // stmt for error;
            }

        }break;
        case TK_IF :{
            NODE_AstTree boolExpr = root->child;
            NODE_AstTree stmts = root->child->sibling;
            NODE_AstTree elsePart = root->child->sibling->sibling;

            if(getExpressionDtype(boolExpr)==BOOL){
                checkStmtType(stmts);
                checkStmtType(elsePart);
            }else{
                // stmt for error;
            }

        }break;
        case TK_READ :{
            NODE_AstTree var = root->child;
            dataType dtVar = getIdentifierDtype(var);
            if(var->tokens->tk->token == TK_ID){
                if(dtVar == RECORD || dtVar == ERROR){
                    // stmt for error
                }else{
                    // there is no type error, HURRAY!
                }
            }else{
                // stmt for error
            }

        }break;
        case TK_WRITE :{
            NODE_AstTree var = root->child;
            if(var->tokens->tk->token == TK_NUM){
                // there is no type error, HURRAY!
            }else if(var->tokens->tk->token == TK_RNUM){
                // there is no type error, HURRAY!
            }else if(var->tokens->tk->token == TK_ID){
                if(dtVar == RECORD || dtVar == ERROR){
                    // stmt for error
                }else{
                    // there is no type error, HURRAY!
                }
            }else{
                // stmt for error
            }
        }break;
        case TK_FUNID :{
            // will
        }break;
        default : {
            // error not handelled
        }break;

    }

}

dataType getIdentifierDtype(NODE_AstTree var, HASHSYMBOL hTable, FN_STACK st){
    if(var->tokens->next == NULL){// single identifier is there
        hashsymbol hVar = hTable[lookupEntry(st->function_name,var->tokens->tk->token,var->tokens->tk->token)];

        if(hVar->entry_ptr->int_no == 1 && hVar->entry_ptr->real_no == 0){ //it is an integer
            return INT;
        }else if(hVar->entry_ptr->int_no == 0 && hVar->entry_ptr->real_no == 1){ // it is a real No
            return REAL;
        }else if ((hVar->entry_ptr->int_no + hVar->entry_ptr->real_no) > 1){ // it is a record type
            return RECORD;
        }else{
            return ERROR;
        }
    }else{// identifier with field id is there
        hashsymbol hVar = hTable[lookupEntry(st->function_name,var->tokens->tk->token,var->tokens->next->tk->token)];
        return hVar->field_ptr->dType;
    }
}

dataType getExpressionDtype(NODE_AstTree root, HASHSYMBOL hTable, FN_STACK st){
    if (root->child == NULL) { // it is an identifier, a leafNode
        return getIdentifierDtype(root, hTable, st);
    }else{  // it has 2 children, and is itself a operator node
        tokenType op = root->tokens->tk->token;
        switch (op) {
            case TK_PLUS:
            case TK_MINUS:
                NODE_AstTree child1 = root->child;
                NODE_AstTree child2 = root->child->sibling;

                dataType dtChild1 = getExpressionDtype(child1);
                dataType dtChild2 = getExpressionDtype(child2);

                if(dtChild1 == dtChild2){
                    if(dtChild1 == RECORD){
                        return matchRecordType(child1,child2,hTable,st);
                    }else{
                        return dtChild1;
                    }
                }else{
                    return ERROR;
                }

            break;

            case TK_MUL:
            case TK_DIV:
                NODE_AstTree child1 = root->child;
                NODE_AstTree child2 = root->child->sibling;

                dataType dtChild1 = getExpressionDtype(child1);
                dataType dtChild2 = getExpressionDtype(child2);

                if(dtChild1 == dtChild2){
                    if(dtChild1 == RECORD){
                        return ERROR;
                    }else{
                        return dtChild1;
                    }
                }else{
                    return ERROR;
                }
            break;

            case TK_NOT:
                NODE_AstTree child1 = root->child;
                return getExpressionDtype(child1);
            break;

            case TK_AND:
            case TK_OR:
                NODE_AstTree child1 = root->child;
                NODE_AstTree child2 = root->child->sibling;

                dataType dtChild1 = getExpressionDtype(child1);
                dataType dtChild2 = getExpressionDtype(child2);

                if(dtChild1 == dtChild2){
                    if(dtChild1 == BOOL){
                        return BOOL;
                    }else{
                        return ERROR;
                    }
                }else{
                    return ERROR;
                }
            break;

            case TK_LT:
            case TK_LE:
            case TK_EQ:
            case TK_GT:
            case TK_GE:
            case TK_NE:
                NODE_AstTree child1 = root->child;
                NODE_AstTree child2 = root->child->sibling;

                dataType dtChild1 = getExpressionDtype(child1);
                dataType dtChild2 = getExpressionDtype(child2);

                if(dtChild1 == dtChild2){
                    if(dtChild1 == INT || dtChild1 == REAL){
                        return dtChild1;
                    }else{
                        return ERROR;
                    }
                }else{
                    return ERROR;
                }
            break;
        }
    }
}

dataType matchRecordType(NODE_AstTree child1, NODE_AstTree child2, HASHSYMBOL hTable, FN_STACK st){
    // go to leaf node of both and check their recordID
    NODE_AstTree tmp1 = child1;
    while (tmp1->child == NULL) {
        tmp1 = tmp1->child;
    }
    NODE_AstTree tmp2 = child2;
    while (tmp2->child == NULL) {
        tmp2 = tmp2->child;
    }

    hashsymbol h1 = hTable[lookupEntry(st->function_name,tmp1->tokens->tk->token,tmp1->tokens->tk->token)];
    hashsymbol h2 = hTable[lookupEntry(st->function_name,tmp2->tokens->tk->token,tmp2->tokens->tk->token)];


    if (strcmp(h1->symbolentry->record_name,h2->symbolentry->record_name)==0) {
        return RECORD;
    }else{
        return ERROR;
    }

}
