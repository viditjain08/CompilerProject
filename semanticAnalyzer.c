#include "semanticAnalyzerDef.h"

void checkInitialisations(NODE_AstTree expr, int funcHashVal){
    if (expr->child == NULL) { // it is an identifier
        ifInit(expr,funcHashVal);
    }else{
        tokenType op = expr->tokens->tk->token;
        if(op == TK_NOT){ // it has one child
            checkInitialisations(expr->child,funcHashVal);
        }else{  // it has two child
            checkInitialisations(expr->child,funcHashVal);
            checkInitialisations(expr->child->sibling,funcHashVal);
        }
    }
}

void ifInit(NODE_AstTree var, int funcHashVal){
    if(var->tokens->tk->token == TK_NUM){
        return ;
    }else if(var->tokens->tk->token == TK_RNUM){
        return ;
    }
    if(var->tokens->next == NULL){// single identifier is there
        int index = getHashIndex(var,funcHashVal);
        if(index < 0){
            return ;
        }
        hashsymbol hVar = h[index];
        if ((hVar.entry_ptr->int_no + hVar.entry_ptr->real_no) > 1 && hVar.entry_ptr->init != 1) { // it is a record, check if all its fields are set or not
            FIELD temp = hVar.entry_ptr->record;
            int flag = 1;
            while (temp!=NULL) {
                if(temp->set != 1){
                    flag = 0;
                    break;
                }
            }
            if (flag) {
                hVar.entry_ptr->init = 1;
            }
        }
        if(hVar.entry_ptr->init != 1) {// it is not initialised
            printf("Line:%d ==> Var %s is not initialised\n",var->tokens->tk->lineNo,var->tokens->tk->lexeme );
            return ;
        }

    }else{// identifier with field id is there
        int index = getHashIndex(var,funcHashVal);
        if(index < 0){
            return ;
        }
        hashsymbol hVar = h[index];

        if (hVar.field_ptr->set != 1) {
            printf("Line:%d ==> Var %s.%s is not initialised\n",var->tokens->tk->lineNo,var->tokens->tk->lexeme,var->tokens->next->tk->lexeme );
            return ;
        }
    }
}

int checkFunctionInvoke(NODE_AstTree stmt, int funcHashVal) {
    int index = getFunction(stmt->tokens->tk->lexeme);
    if(index < 0){
        // error, function not defined
        printf("Line:%d ==> Function %s not defined\n",stmt->tokens->tk->lineNo,stmt->tokens->tk->lexeme );
        return -1;
    }
    int newFuncInd = functions[index].index;
    int oldFuncInd = functions[funcHashVal].index;

    if(oldFuncInd <= newFuncInd){
        // stmt for error, old does not know the signature of new function
        printf("Line:%d ==> %s does not know the signature of %s\n", stmt->tokens->tk->lineNo,functions[funcHashVal].function_name,functions[index].function_name);
    }

    return oldFuncInd-newFuncInd;

}

int getHashIndex(NODE_AstTree var, int funcHashVal){
    int index;
    if(var->tokens->next == NULL){// single identifier is there
        index = lookupEntry("global",var->tokens->tk->lexeme,NULL);
        if(index < 0){
            // error, var not defined
            index = lookupEntry(functions[funcHashVal].function_name,var->tokens->tk->lexeme,NULL);
            return index;
        }

    }else{// identifier with field id is there
        index = lookupEntry("global",var->tokens->tk->lexeme,var->tokens->next->tk->lexeme);
        if(index < 0){
            // error, var not defined
            index = lookupEntry(functions[funcHashVal].function_name,var->tokens->tk->lexeme,var->tokens->next->tk->lexeme);
            if(index < 0){
                // error, var not defined
                return index;
            }
        }
    }
}



void semStmts(NODE_AstTree root, int funcHashVal){
    NODE_AstTree stmt = root->child;

    while (stmt != NULL) {


        tokenType tok = stmt->tokens->tk->token;

        switch (tok) {
            case TK_ASSIGNOP:{
                //
                if(assignStmtSemantics(stmt,funcHashVal) == ERROR){
                    stmt = stmt->sibling;
                    break;
                }
                checkInitialisations(stmt->child->sibling,funcHashVal);

                if(getHashIndex(stmt->child,funcHashVal) > 0){
                    h[getHashIndex(stmt->child,funcHashVal)].entry_ptr->init = 1;
                }


            }
            break;

            case TK_WHILE:{
                NODE_AstTree boolExpr = stmt->child;
                NODE_AstTree stmts = stmt->child->sibling;
                dataType dt = getExpressionDtype(boolExpr,funcHashVal);
                if(dt == BOOL){ // no type errors in boolExpr
                    checkInitialisations(boolExpr,funcHashVal);
                }else if(dt != ERROR){
                    // don't check Initialisations
                    printf("Line:%d ==> Data type of booleanExpression is not bool\n",boolExpr->tokens->tk->lineNo );
                }
                semStmts(stmts,funcHashVal);
                whileLoopSemantic(stmt,funcHashVal);
            }
            break;

            case TK_IF:{
                NODE_AstTree boolExpr = stmt->child;
                NODE_AstTree stmts = stmt->child->sibling;
                NODE_AstTree elsePart = stmt->child->sibling->sibling;

                if(getExpressionDtype(boolExpr,funcHashVal)==BOOL){
                    checkInitialisations(boolExpr,funcHashVal);
                }else{
                    // stmt for error;
                    printf("Line:%d ==> Data type of booleanExpression is not bool\n",boolExpr->tokens->tk->lineNo );
                }
                semStmts(stmts,funcHashVal);
                semStmts(elsePart,funcHashVal);
            }
            break;

            case TK_READ:{
                if(getIdentifierDtype(stmt->child,funcHashVal) == RECORD){
                    printf("Line:%d ==> Data type of %s is not valid for reading input\n",stmt->child->tokens->tk->lineNo,stmt->child->tokens->tk->lexeme );
                    break;
                }
                if(getHashIndex(stmt->child,funcHashVal) > 0){
                    h[getHashIndex(stmt->child,funcHashVal)].entry_ptr->init = 1;
                }
            }
            break;

            case TK_WRITE:{
                dataType dtVar = getIdentifierDtype(stmt->child,funcHashVal);
                if (stmt->child->tokens->tk->token == TK_ID && dtVar == RECORD) {
                    printf("Line:%d ==> Data type of %s is not valid for reading input\n",stmt->child->tokens->tk->lineNo,stmt->child->tokens->tk->lexeme );
                }else if(stmt->child->tokens->tk->token == TK_ID && dtVar != RECORD){
                    checkInitialisations(stmt->child,funcHashVal);
                }
            }
            break;

            case TK_FUNID:{
                // type error out to out- in to in ,
                // function invoke error

                NODE_AstTree outputParList = stmt->child;
                NODE_AstTree inputParList = stmt->child->sibling;

                NODE_AstTree func = functions[funcHashVal].ptr;
                NODE_AstTree inputParSignature = func->child->sibling;
                NODE_AstTree outputParSignature = func->child->sibling->sibling;

                int ifInit = 1;
                switch (checkFunctionSignature(inputParList,inputParSignature,funcHashVal)) {
                    case 1:{ // types don't match
                        ifInit = 0;
                    }break;

                    case 2:{ // numbers don't match
                        ifInit = 0;
                    }break;

                    case 3:{ // both types and numbers don't match
                        ifInit = 0;
                    }break;
                }

                switch (checkFunctionSignature(outputParList,outputParSignature,funcHashVal)) {
                    case 1:{ // types don't match
                        ifInit = 0;
                    }break;

                    case 2:{ // numbers don't match
                        ifInit = 0;
                    }break;

                    case 3:{ // both types and numbers don't match
                        ifInit = 0;
                    }break;
                }

                if ((checkFunctionInvoke(stmt, funcHashVal)) > 0 && (ifInit == 1)) {
                    // initialise all the variables in outputParList
                    NODE_AstTree tmp = outputParList->child;
                    while (tmp!= NULL) {
                        h[getHashIndex(tmp,funcHashVal)].entry_ptr->init = 1;
                        tmp = tmp->sibling;
                    }
                }

            }
            break;
        }
        stmt = stmt->sibling;

    }
}

int assignStmtSemantics(NODE_AstTree root, int funcHashVal){
    NODE_AstTree var = root->child;
    NODE_AstTree arExpr = root->child->sibling;

    dataType dtVar = getIdentifierDtype(var, funcHashVal);
    dataType dtArExpr = getExpressionDtype(arExpr, funcHashVal);
    if(dtVar == dtArExpr){
        if(dtVar == RECORD){// we have to matchRecordType
            return (matchRecordType(var,arExpr,funcHashVal) == RECORD) ? 1 : 0;
        }else{
            // there is no type error, HURRAY!
            return 1;
        }
    }else if(dtVar != ERROR && dtArExpr != ERROR){
        // stmt for error, they are valid but types don't match
        printf("Line:%d ==> Types of LHS and RHS of assignment stmt don't match\n", var->tokens->tk->lineNo);
        return 0;
    }
}

void returnStmtSemantics(NODE_AstTree stmt, int funcHashVal) {

}

void whileLoopSemantic(NODE_AstTree stmt, int funcHashVal){

}

int checkFunctionSignature(NODE_AstTree par_list, NODE_AstTree par_signature, int funcHashVal){
    // this function will return 0 if no error, 1 if numbers don't match, 2 if types don't match, 3 if both don't match

    NODE_AstTree tmp1 = par_list->child;
    NODE_AstTree tmp2 = par_signature->child;
    int res = 0;
    while(tmp1 != NULL && tmp2 != NULL){
        dataType dt2 ;
        if(strcmp(tmp2->tokens->tk->lexeme,"int")==0){
            dt2 = INT;
        }else if(strcmp(tmp2->tokens->tk->lexeme,"real")==0){
            dt2 = REAL;
        }else{
            dt2 = RECORD;
        }
        dataType dt1 = getIdentifierDtype(tmp1,funcHashVal);
        if (dt1 != dt2) {
            res = 1;
            break;
        }else if(dt1 == RECORD){
            if (strcmp(h[getHashIndex(tmp1,funcHashVal)].entry_ptr->record_name,tmp2->tokens->tk->lexeme) != 0) {
                res= 1;
                break;
            }
        }
    }

    if(tmp1 == NULL && tmp2 != NULL){
        res = (res == 1) ? 3 : 2;
    }else if(tmp1 != NULL && tmp2 == NULL){
        res = (res == 1) ? 3 : 2;
    }else{
        // HURRAY! No error of number of parameters
    }

    return res;


}

void semAnalyze(NODE_AstTree root){
    NODE_AstTree tmp = root->child->sibling;
    while(tmp!=NULL){
        int funcHashVal = getFunction(tmp->child->tokens->tk->lexeme);
        NODE_AstTree node = tmp->child->sibling->sibling->sibling->child->sibling->sibling;
        semStmts(node,funcHashVal);
        returnStmtSemantics(node->sibling,funcHashVal);
        tmp = tmp->sibling;
    }
    semStmts(root->child->child->sibling->sibling,getFunction("_main"));

}
