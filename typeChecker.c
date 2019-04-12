#include "typeCheckerDef.h"

void checkStmtsType(NODE_AstTree root, int funcHashVal){
    NODE_AstTree stmt = root->child;

    while (stmt != NULL) {
        checkStmtType(stmt, funcHashVal);
        stmt = stmt->sibling;
    }
}

void checkStmtType(NODE_AstTree root, int funcHashVal){
    tokenType tok = root->tokens->tk->token;

    switch (tok) {
        case TK_ASSIGNOP :{
            NODE_AstTree var = root->child;
            NODE_AstTree arExpr = root->child->sibling;

            dataType dtVar = getIdentifierDtype(var, funcHashVal);
            dataType dtArExpr = getExpressionDtype(arExpr, funcHashVal);
            if(dtVar == dtArExpr){
                if(dtVar == RECORD){// we have to matchRecordType
                    dataType dt = matchRecordType(var,arExpr,funcHashVal);
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
            if(getExpressionDtype(boolExpr,funcHashVal)==BOOL){
                checkStmtType(stmts,funcHashVal);
            }else{
                // stmt for error;
            }

        }break;
        case TK_IF :{
            NODE_AstTree boolExpr = root->child;
            NODE_AstTree stmts = root->child->sibling;
            NODE_AstTree elsePart = root->child->sibling->sibling;

            if(getExpressionDtype(boolExpr,funcHashVal)==BOOL){
                checkStmtType(stmts,funcHashVal);
                checkStmtType(elsePart,funcHashVal);
            }else{
                // stmt for error;
            }

        }break;
        case TK_READ :{
            NODE_AstTree var = root->child;
            dataType dtVar = getIdentifierDtype(var,funcHashVal);
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
                dataType dtVar = getIdentifierDtype(var,funcHashVal);
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
            // will check number and types of input and output parameters and return Stmt
            NODE_AstTree func = functions[funcHashVal].ptr;
            NODE_AstTree inputPar = func->child->sibling;
            NODE_AstTree outputPar = func->child->sibling->sibling;
            NODE_AstTree returnType = func->child->sibling->sibling->sibling->child->sibling->sibling->sibling;

            // firstly check output type and return type of function
            NODE_AstTree tmp1 = outputPar->child;
            NODE_AstTree tmp2 = returnType->child;
            int flag = 1;
            while(tmp1 != NULL && tmp2 != NULL){
                dataType dt1 ;
                if(strcmp(tmp1->tokens->next->tk->lexeme,"int")==0){
                    dt1 = INT;
                }else if(strcmp(tmp1->tokens->next->tk->lexeme,"real")==0){
                    dt1 = REAL;
                }else{
                    dt1 = RECORD;
                }
                dataType dt2 = getIdentifierDtype(tmp2,funcHashVal);
                if(dt1 != dt2){// ERROR
                    // stmt for error
                    flag = 0;
                    break;
                }else if(dt1 == RECORD){
                    // we have to match record name
                    int ind = lookupEntry(functions[funcHashVal].function_name,tmp2->tokens->tk->lexeme,tmp2->tokens->tk->lexeme);
                    if(ind < 0){
                        // error, var not defined
                        return;
                    }
                    char *str = h[ind].entry_ptr->record_name;
                    if(strcmp(tmp1->tokens->tk->lexeme,str)!=0){
                        // stmt for error
                        flag = 0;
                        break;
                    }
                }
            }
            if(tmp1 == NULL && tmp2 != NULL){
                // tmp1 has more params, ERROR
                flag = 0;
            }else if(tmp1 != NULL && tmp2 == NULL){
                // tmp2 has more params, ERROR
                flag = 0;
            }else{
                // HURRAY! No error of number of parameters
            }

            // now check ouput type of fun id with that of function
            if(flag){ // check only if flag = 1 as output type and return type should match
                tmp1 = outputPar->child;
                tmp2 = root->child;
                while(tmp1 != NULL && tmp2 != NULL){ // check for data types
                    dataType dt1 ;
                    if(strcmp(tmp1->tokens->next->tk->lexeme,"int")==0){
                        dt1 = INT;
                    }else if(strcmp(tmp1->tokens->next->tk->lexeme,"real")==0){
                        dt1 = REAL;
                    }else{
                        dt1 = RECORD;
                    }
                    dataType dt2 = getIdentifierDtype(tmp2,funcHashVal);
                    if(dt1 != dt2){// ERROR
                        // stmt for error

                        break;
                    }else if(dt1 == RECORD){
                        // we have to match record name
                        int ind = lookupEntry(functions[funcHashVal].function_name,tmp2->tokens->tk->lexeme,tmp2->tokens->tk->lexeme);
                        if(ind < 0){
                            // error, var not defined
                            return;
                        }
                        char *str = h[ind].entry_ptr->record_name;
                        if(strcmp(tmp1->tokens->tk->lexeme,str)!=0){
                            // stmt for error
                            break;
                        }
                    }
                }
                if(tmp1 == NULL && tmp2 != NULL){
                    // tmp1 has more params, ERROR
                }else if(tmp1 != NULL && tmp2 == NULL){
                    // tmp2 has more params, ERROR
                }else{
                    // HURRAY! No error of number of parameters
                }

            }
            // now check input type of fun id with that of function
            tmp1 = inputPar->child;
            tmp2 = root->child->sibling;
            while(tmp1 != NULL && tmp2 != NULL){ // check for data types
                dataType dt1 ;
                if(strcmp(tmp1->tokens->next->tk->lexeme,"int")==0){
                    dt1 = INT;
                }else if(strcmp(tmp1->tokens->next->tk->lexeme,"real")==0){
                    dt1 = REAL;
                }else{
                    dt1 = RECORD;
                }
                dataType dt2 = getIdentifierDtype(tmp2,funcHashVal);
                if(dt1 != dt2){// ERROR
                    // stmt for error

                    break;
                }else if(dt1 == RECORD){
                    // we have to match record name
                    int ind = lookupEntry(functions[funcHashVal].function_name,tmp2->tokens->tk->lexeme,tmp2->tokens->tk->lexeme);
                    if(ind < 0){
                        // error, var not defined
                        return;
                    }
                    char *str = h[ind].entry_ptr->record_name;
                    if(strcmp(tmp1->tokens->tk->lexeme,str)!=0){
                        // stmt for error
                        break;
                    }
                }
            }
            if(tmp1 == NULL && tmp2 != NULL){
                // tmp1 has more params, ERROR
            }else if(tmp1 != NULL && tmp2 == NULL){
                // tmp2 has more params, ERROR
            }else{
                // HURRAY! No error of number of parameters
            }

        }break;
        default : {
            // error not handelled
        }break;

    }

}

dataType getIdentifierDtype(NODE_AstTree var, int funcHashVal){
    int index ;
    if(var->tokens->tk->token == TK_NUM){
        return INT;
    }else if(var->tokens->tk->token == TK_RNUM){
        return REAL;
    }
    if(var->tokens->next == NULL){// single identifier is there
        index = lookupEntry(functions[funcHashVal].function_name,var->tokens->tk->lexeme,var->tokens->tk->lexeme);
        if(index < 0){
            // error, var not defined
            return ERROR;
        }
        hashsymbol hVar = h[index];

        if(hVar.entry_ptr->int_no == 1 && hVar.entry_ptr->real_no == 0){ //it is an integer
            return INT;
        }else if(hVar.entry_ptr->int_no == 0 && hVar.entry_ptr->real_no == 1){ // it is a real No
            return REAL;
        }else if ((hVar.entry_ptr->int_no + hVar.entry_ptr->real_no) > 1){ // it is a record type
            return RECORD;
        }else{
            return ERROR;
        }
    }else{// identifier with field id is there
        index = lookupEntry(functions[funcHashVal].function_name,var->tokens->tk->lexeme,var->tokens->next->tk->lexeme);
        if(index < 0){
            // error, var not defined
            return ERROR;
        }
        hashsymbol hVar = h[index];

        return hVar.field_ptr->dType;
    }
}

dataType getExpressionDtype(NODE_AstTree root, int funcHashVal){
    if (root->child == NULL) { // it is an identifier, a leafNode
        return getIdentifierDtype(root,funcHashVal);
    }else{  // it has 2 children, and is itself a operator node
        tokenType op = root->tokens->tk->token;
        switch (op) {
            case TK_PLUS:
            case TK_MINUS:{
                NODE_AstTree child1 = root->child;
                NODE_AstTree child2 = root->child->sibling;

                dataType dtChild1 = getExpressionDtype(child1,funcHashVal);
                dataType dtChild2 = getExpressionDtype(child2,funcHashVal);

                if(dtChild1 == dtChild2){
                    if(dtChild1 == RECORD){
                        return matchRecordType(child1,child2,funcHashVal);
                    }else{
                        return dtChild1;
                    }
                }else{
                    return ERROR;
                }
            }
            break;

            case TK_MUL:
            case TK_DIV:{
                NODE_AstTree child1 = root->child;
                NODE_AstTree child2 = root->child->sibling;

                dataType dtChild1 = getExpressionDtype(child1,funcHashVal);
                dataType dtChild2 = getExpressionDtype(child2,funcHashVal);

                if(dtChild1 == dtChild2){
                    if(dtChild1 == RECORD){
                        return ERROR;
                    }else{
                        return dtChild1;
                    }
                }else{
                    return ERROR;
                }
            }
            break;

            case TK_NOT:{

                NODE_AstTree child1 = root->child;
                return getExpressionDtype(child1,funcHashVal);
            }
            break;

            case TK_AND:
            case TK_OR:{
                NODE_AstTree child1 = root->child;
                NODE_AstTree child2 = root->child->sibling;

                dataType dtChild1 = getExpressionDtype(child1,funcHashVal);
                dataType dtChild2 = getExpressionDtype(child2,funcHashVal);

                if(dtChild1 == dtChild2){
                    if(dtChild1 == BOOL){
                        return BOOL;
                    }else{
                        return ERROR;
                    }
                }else{
                    return ERROR;
                }
            }
            break;

            case TK_LT:
            case TK_LE:
            case TK_EQ:
            case TK_GT:
            case TK_GE:
            case TK_NE:{
                NODE_AstTree child1 = root->child;
                NODE_AstTree child2 = root->child->sibling;

                dataType dtChild1 = getExpressionDtype(child1,funcHashVal);
                dataType dtChild2 = getExpressionDtype(child2,funcHashVal);

                if(dtChild1 == dtChild2){
                    if(dtChild1 == INT || dtChild1 == REAL){
                        return dtChild1;
                    }else{
                        return ERROR;
                    }
                }else{
                    return ERROR;
                }
            }
            break;
        }
    }
}

dataType matchRecordType(NODE_AstTree child1, NODE_AstTree child2,int funcHashVal){
    // go to leaf node of both and check their recordID
    NODE_AstTree tmp1 = child1;
    while (tmp1->child == NULL) {
        tmp1 = tmp1->child;
    }
    NODE_AstTree tmp2 = child2;
    while (tmp2->child == NULL) {
        tmp2 = tmp2->child;
    }
    int ind1 = lookupEntry(functions[funcHashVal].function_name,tmp1->tokens->tk->lexeme,tmp1->tokens->tk->lexeme);
    if(ind1 < 0){
        // error, var not defined
        return ERROR;
    }
    int ind2 = lookupEntry(functions[funcHashVal].function_name,tmp2->tokens->tk->lexeme,tmp2->tokens->tk->lexeme);
    if(ind2 < 0){
        // error, var not defined
        return ERROR;
    }
    hashsymbol h1 = h[ind1];
    hashsymbol h2 = h[ind2];


    if (strcmp(h1.entry_ptr->record_name,h2.entry_ptr->record_name)==0) {
        return RECORD;
    }else{
        return ERROR;
    }

}

void checkFunctionInvoke(NODE_AstTree stmt, int funcHashVal) {
    int index = getFunction(stmt->tokens->tk->lexeme);
    if(index < 0){
        // error, function not defined
        return ;
    }
    int newFuncInd = functions[index].index;
    int oldFuncInd = functions[funcHashVal].index;

    if(oldFuncInd <= newFuncInd){
        // stmt for error, old does not know the signature of new function
    }

}

void checkType(NODE_AstTree root){
    NODE_AstTree tmp = root->child->sibling;
    while(tmp!=NULL){
        int funcHashVal = getFunction(tmp->child->tokens->tk->lexeme);
        NODE_AstTree node = tmp->child->sibling->sibling->sibling->child->sibling->sibling;
        checkStmtsType(node,funcHashVal);
        tmp = tmp->sibling;
    }
    checkStmtsType(root->child->child->sibling->sibling->sibling,getFunction("_main"));
}
