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
        hashsymbol hash = h[lookupEntry(functions[funcHashVal].function_name,var->tokens->tk->lexeme,NULL)];
        if (hash.entry_ptr->init == 1) {
            FIELD tmp = hash.entry_ptr->record;
            while (tmp != NULL) {
                tmp->set = 1;
                tmp = tmp->next;
            }
        }

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
                if(assignStmtSemantics(stmt,funcHashVal) == 0){
                    printf("Line:%d ==> Data Types don't match in assignment statement\n",stmt->child->tokens->tk->lineNo );
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
                char* dt = getExpressionDtype(boolExpr,funcHashVal);
                if(!(strcmp(dt,"bool"))){ // no type errors in boolExpr
                    checkInitialisations(boolExpr,funcHashVal);
                }else if(strcmp(dt,"error")){
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

                char* dt = getExpressionDtype(boolExpr,funcHashVal);
                if(!(strcmp(dt,"bool"))){ // no type errors in boolExpr
                    checkInitialisations(boolExpr,funcHashVal);
                }else if(strcmp(dt,"error")){
                    // don't check Initialisations
                    printf("Line:%d ==> Data type of booleanExpression is not bool\n",boolExpr->tokens->tk->lineNo );
                }
                semStmts(stmts,funcHashVal);
                semStmts(elsePart,funcHashVal);
            }
            break;

            case TK_READ:{
                char *dt = getIdentifierDtype(stmt->child,funcHashVal);
                if(strcmp(dt,"int") && strcmp(dt,"real") && strcmp(dt,"error")){
                    printf("Line:%d ==> Data type of %s is not valid for reading input\n",stmt->child->tokens->tk->lineNo,stmt->child->tokens->tk->lexeme );
                    break;
                }
                if(getHashIndex(stmt->child,funcHashVal) > 0){
                    h[getHashIndex(stmt->child,funcHashVal)].entry_ptr->init = 1;
                }
            }
            break;

            case TK_WRITE:{
                char* dtVar = getIdentifierDtype(stmt->child,funcHashVal);
                if (stmt->child->tokens->tk->token == TK_ID && strcmp(dtVar,"int") && strcmp(dtVar,"real") && strcmp(dtVar,"error")) {
                    printf("Line:%d ==> Data type of %s is not valid for writing output\n",stmt->child->tokens->tk->lineNo,stmt->child->tokens->tk->lexeme );
                    break;
                }else if(strcmp(dtVar,"error")){
                    checkInitialisations(stmt->child,funcHashVal);
                }

            }
            break;

            case TK_FUNID:{
                // type error out to out- in to in ,
                // function invoke error

                NODE_AstTree outputParList = stmt->child;
                NODE_AstTree inputParList = stmt->child->sibling;

                NODE_AstTree func = functions[getFunction(stmt->tokens->tk->lexeme)].ptr;
                NODE_AstTree inputParSignature = func->child->sibling;
                NODE_AstTree outputParSignature = func->child->sibling->sibling;

                int init = 1;
                switch (checkFunctionSignature(inputParList,inputParSignature,funcHashVal)) {
                    case 1:{ // types don't match
                        init = 0;
                        printf("Line:%d ==> types of input parameters of call function %s don't match with function signature\n",stmt->tokens->tk->lineNo,stmt->tokens->tk->lexeme );
                    }break;

                    case 2:{ // numbers don't match
                        init = 0;
                        printf("Line:%d ==> number of input parameters of call function %s don't match with function signature\n",stmt->tokens->tk->lineNo,stmt->tokens->tk->lexeme );

                    }break;

                    case 3:{ // both types and numbers don't match
                        init = 0;
                        printf("Line:%d ==> number and types of input parameters of call function %s don't match with function signature\n",stmt->tokens->tk->lineNo,stmt->tokens->tk->lexeme );

                    }break;
                }

                switch (checkFunctionSignature(outputParList,outputParSignature,funcHashVal)) {
                    case 1:{ // types don't match
                        init = 0;
                        printf("Line:%d ==> types of output parameters of call function %s don't match with function signature\n",stmt->tokens->tk->lineNo,stmt->tokens->tk->lexeme );
                    }break;

                    case 2:{ // numbers don't match
                        init = 0;
                        printf("Line:%d ==> number of output parameters of call function %s don't match with function signature\n",stmt->tokens->tk->lineNo,stmt->tokens->tk->lexeme );

                    }break;

                    case 3:{ // both types and numbers don't match
                        init = 0;
                        printf("Line:%d ==> number and types of output parameters of call function %s don't match with function signature\n",stmt->tokens->tk->lineNo,stmt->tokens->tk->lexeme );

                    }break;
                }

                if ((checkFunctionInvoke(stmt, funcHashVal)) > 0 && (init == 1)) {
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

    char* dtVar = getIdentifierDtype(var, funcHashVal);
    char* dtArExpr = getExpressionDtype(arExpr, funcHashVal);
    if(!strcmp(dtVar,dtArExpr) && strcmp(dtVar,"error") && strcmp(dtArExpr,"error")){
        return 1;
    }else{//else if(strcmp(dtVar,"error") && strcmp(dtArExpr,"error")){
        // stmt for error, they are valid but types don't match
        // printf("Line:%d ==> Types of LHS and RHS of assignment stmt don't match\n", var->tokens->tk->lineNo);
        return 0;
    }
}

void returnStmtSemantics(NODE_AstTree stmt, int funcHashVal) {
    NODE_AstTree outputSignature = functions[funcHashVal].ptr->child->sibling->sibling;
    // first check type errors
    int flag = 1;
    switch (checkFunctionSignature(stmt,outputSignature,funcHashVal)) {
        case 1:{
            printf("Line:%d ==> types of return parameters of function %s don't match with function signature\n",stmt->child->tokens->tk->lineNo,functions[funcHashVal].function_name );
            flag = 0;
        }break;
        case 2:{
            printf("Line:%d ==> number of return parameters of function %s don't match with function signature\n",stmt->child->tokens->tk->lineNo,functions[funcHashVal].function_name );
            flag = 0;
        }break;
        case 3:{
            printf("Line:%d ==> types and number of return parameters of function %s don't match with function signature\n",stmt->child->tokens->tk->lineNo,functions[funcHashVal].function_name );
            flag = 0;
        }break;
    }
    if (flag) { //if no type error then see if every parameter is assigned vaue is not
        NODE_AstTree val = stmt->child;
        while(val!=NULL){
            if(h[getHashIndex(val,funcHashVal)].entry_ptr->init != 1){
                printf("Line:%d ==> %s parameter is not assigned any value\n",val->tokens->tk->lineNo,val->tokens->tk->lexeme );
                return ;
            }
            val = val->sibling;
        }
    }
}

void whileLoopSemantic(NODE_AstTree stmt, int funcHashVal){

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

int checkFunctionSignature(NODE_AstTree par_list, NODE_AstTree par_signature, int funcHashVal){
    // this function will return 0 if no error, 1 if numbers don't match, 2 if types don't match, 3 if both don't match

    NODE_AstTree tmp1 = par_list->child;
    NODE_AstTree tmp2 = par_signature->child;
    int res = 0;
    while(tmp1 != NULL && tmp2 != NULL){
        char *dt2 = tmp2->tokens->tk->lexeme;

        char *dt1 = getIdentifierDtype(tmp1,funcHashVal);
        if (strcmp(dt1,dt2)) {
            res = 1;
            break;
        }
        tmp1 = tmp1->sibling;
        tmp2 = tmp2->sibling;
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
    initInputParameters(root);
    NODE_AstTree tmp = root->child->sibling;
    while(tmp!=NULL){
        int funcHashVal = getFunction(tmp->child->tokens->tk->lexeme);
        NODE_AstTree node = tmp->child->sibling->sibling->sibling->child->sibling->sibling;
        semStmts(node,funcHashVal);
        returnStmtSemantics(node->sibling,funcHashVal);
        tmp = tmp->sibling;
    }
    int n = getFunction("_main");
    semStmts(root->child->child->sibling->sibling,n);

}

void initInputParameters(NODE_AstTree root) {
    NODE_AstTree tmp = root->child->sibling;

    while (tmp!=NULL) {
        NODE_AstTree inp = tmp->child->sibling->child;

        while (inp!= NULL) {
            h[lookupEntry(tmp->child->tokens->tk->lexeme,inp->tokens->next->tk->lexeme,NULL)].entry_ptr->init = 1;
            inp = inp->sibling;
        }
        tmp =tmp->sibling;

    }

}
