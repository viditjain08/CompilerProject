#include "semanticAnalyzerDef.h"

char buf[256];

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
            memset(buf,0,256);
            sprintf(buf,"Line:%d ==> Var %s is not initialised\n",var->tokens->tk->lineNo,var->tokens->tk->lexeme );
            addError(var->tokens->tk->lineNo);
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
            memset(buf,0,256);
            sprintf(buf,"Line:%d ==> Var %s.%s is not initialised\n",var->tokens->tk->lineNo,var->tokens->tk->lexeme,var->tokens->next->tk->lexeme );
            addError(var->tokens->tk->lineNo);
            return ;
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
                    if (errno != 1) {
                        memset(buf,0,256);
                        sprintf(buf,"Line:%d ==> Data Types don't match in assignment statement\n",stmt->child->tokens->tk->lineNo );
                        addError(stmt->child->tokens->tk->lineNo);
                    }
                    errno = 0;
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
                    whileLoopSemantic(stmt,funcHashVal);
                }else if(strcmp(dt,"error")){
                    // don't check Initialisations
                    memset(buf,0,256);
                    sprintf(buf,"Line:%d ==> Data type of booleanExpression is not bool\n",boolExpr->tokens->tk->lineNo );
                    addError(boolExpr->tokens->tk->lineNo);
                }
                semStmts(stmts,funcHashVal);

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
                    memset(buf,0,256);
                    sprintf(buf,"Line:%d ==> Data type of booleanExpression is not bool\n",boolExpr->tokens->tk->lineNo );
                    addError(boolExpr->tokens->tk->lineNo);
                }
                semStmts(stmts,funcHashVal);
                semStmts(elsePart,funcHashVal);
            }
            break;

            case TK_READ:{
                char *dt = getIdentifierDtype(stmt->child,funcHashVal);
                if(stmt->child->tokens->tk->token != TK_ID){
                    memset(buf,0,256);
                    sprintf(buf,"Line:%d ==> Data type of %s is not valid for reading input\n",stmt->child->tokens->tk->lineNo,stmt->child->tokens->tk->lexeme );
                    addError(stmt->child->tokens->tk->lineNo);
                    break;
                }
                if(getHashIndex(stmt->child,funcHashVal) > 0 && errno != 1){
                    h[getHashIndex(stmt->child,funcHashVal)].entry_ptr->init = 1;
                }
                errno = 0;
            }
            break;

            case TK_WRITE:{
                char* dtVar = getIdentifierDtype(stmt->child,funcHashVal);
                if (stmt->child->tokens->tk->token != TK_ID && stmt->child->tokens->tk->token != TK_NUM && stmt->child->tokens->tk->token != TK_RNUM) {
                    memset(buf,0,256);
                    sprintf(buf,"Line:%d ==> var %s is not valid for writing output\n",stmt->child->tokens->tk->lineNo,stmt->child->tokens->tk->lexeme );
                    addError(stmt->child->tokens->tk->lineNo);
                    break;
                }else if(errno != 1){
                    checkInitialisations(stmt->child,funcHashVal);
                }
                errno = 0;

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
                        if (errno != 1 && (errno == 2 || errno == 3)) {
                            memset(buf,0,256);
                            sprintf(buf,"Line:%d ==> Input parameters type mismatch\n",stmt->tokens->tk->lineNo );
                            addError(stmt->tokens->tk->lineNo );
                        }
                        errno = 0;
                    }break;

                    case 2:{ // numbers don't match
                        init = 0;
                        if (errno != 1 && (errno == 2 || errno == 3)) {
                            memset(buf,0,256);
                            sprintf(buf,"Line:%d ==> Input parameters number mismatch\n",stmt->tokens->tk->lineNo );
                            addError(stmt->tokens->tk->lineNo );
                        }
                        errno = 0;

                    }break;

                    case 3:{ // both types and numbers don't match
                        init = 0;
                        if (errno != 1 && (errno == 2 || errno == 3)) {
                            memset(buf,0,256);
                            sprintf(buf,"Line:%d ==> Input parameters type and number mismatch\n",stmt->tokens->tk->lineNo );
                            addError(stmt->tokens->tk->lineNo );
                        }
                        errno = 0;

                    }break;
                }

                switch (checkFunctionSignature(outputParList,outputParSignature,funcHashVal)) {
                    case 1:{ // types don't match
                        init = 0;
                        if (errno != 1 && (errno == 2 || errno == 3)) {
                            memset(buf,0,256);
                            sprintf(buf,"Line:%d ==> output parameters type mismatch\n",stmt->tokens->tk->lineNo );
                            addError(stmt->tokens->tk->lineNo );
                        }
                        errno = 0;
                    }break;

                    case 2:{ // numbers don't match
                        init = 0;
                        if (errno != 1 && (errno == 2 || errno == 3)) {
                            memset(buf,0,256);
                            sprintf(buf,"Line:%d ==> output parameters number mismatch\n",stmt->tokens->tk->lineNo);
                            addError(stmt->tokens->tk->lineNo );
                        }
                        errno = 0;

                    }break;

                    case 3:{ // both types and numbers don't match
                        init = 0;
                        if (errno != 1 && (errno == 2 || errno == 3)) {
                            memset(buf,0,256);
                            sprintf(buf,"Line:%d ==> output parameters type and number mismatch\n",stmt->tokens->tk->lineNo);
                            addError(stmt->tokens->tk->lineNo );
                        }
                        errno = 0;

                    }break;
                }

                if ((checkFunctionInvoke(stmt, funcHashVal)) > 0 && (init == 1)) {
                    // initialise all the variables in outputParList
                    NODE_AstTree tmp = outputParList->child;
                    int ind;
                    while (tmp!= NULL) {
                        ind = getHashIndex(tmp,funcHashVal);
                        if (ind>0) {
                            h[getHashIndex(tmp,funcHashVal)].entry_ptr->init = 1;
                        }

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
            if (errno != 1 && (errno == 2 || errno == 3)) {
                memset(buf,0,256);
                sprintf(buf,"Line:%d ==> return parameter type mismatch in function %s\n",stmt->child->tokens->tk->lineNo,functions[funcHashVal].function_name );
                addError(stmt->child->tokens->tk->lineNo );
            }
            errno = 0;
            flag = 0;
        }break;
        case 2:{
            if (errno != 1 && (errno == 2 || errno == 3)) {
                memset(buf,0,256);
                sprintf(buf,"Line:%d ==> return parameter type mismatch in function %s\n",stmt->child->tokens->tk->lineNo,functions[funcHashVal].function_name );
                addError(stmt->child->tokens->tk->lineNo );
            }
            errno = 0;
            flag = 0;
        }break;
        case 3:{
            if (errno != 1 && (errno == 2 || errno == 3)) {
                memset(buf,0,256);
                sprintf(buf,"Line:%d ==> return parameter type mismatch in function %s\n",stmt->child->tokens->tk->lineNo,functions[funcHashVal].function_name );
                addError(stmt->child->tokens->tk->lineNo );
            }
            errno = 0;
            flag = 0;
        }break;
    }
    if (flag) { //if no type error then see if every parameter is assigned vaue is not
        NODE_AstTree val = stmt->child;
        int ind ;
        while(val!=NULL){
            ind = getHashIndex(val,funcHashVal);
            if(ind> 0 && h[ind].entry_ptr->init != 1){
                memset(buf,0,256);
                sprintf(buf,"Line:%d ==> %s parameter is not assigned any value\n",val->tokens->tk->lineNo,val->tokens->tk->lexeme );
                addError(val->tokens->tk->lineNo );
                return ;
            }
            val = val->sibling;
        }
    }
}
//
// void whileLoopSemantic(NODE_AstTree stmt, int funcHashVal){
//
// }

int checkFunctionInvoke(NODE_AstTree stmt, int funcHashVal) {
    int index = getFunction(stmt->tokens->tk->lexeme);
    if(index < 0){
        // error, function not defined
        memset(buf,0,256);
        sprintf(buf,"Line:%d ==> Function %s not defined\n",stmt->tokens->tk->lineNo,stmt->tokens->tk->lexeme );
        addError(stmt->tokens->tk->lineNo );
        return -1;
    }
    int newFuncInd = functions[index].index;
    int oldFuncInd = functions[funcHashVal].index;

    if(oldFuncInd <= newFuncInd){
        // stmt for error, old does not know the signature of new function
        memset(buf,0,256);
        sprintf(buf,"Line:%d ==> %s can't call function %s\n", stmt->tokens->tk->lineNo,functions[funcHashVal].function_name,functions[index].function_name);
        addError(stmt->tokens->tk->lineNo );
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

    if(errno != 1 && res == 1){
        errno = 2;
    }
    // printf("%d\n",errno );
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
    errno = 0;
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

void addError(int lineNo){
    if (errors[lineNo]!=NULL) {
        // append the string
        // printf("%s", buf);
        char *temp = (char*)malloc(strlen(errors[lineNo])+strlen(buf));
        temp[0] = '\0';
        strcat(temp,errors[lineNo]);
        strcat(temp,buf);
        // printf("%s\n",temp );
        char *tmp = errors[lineNo];
        errors[lineNo] = temp;
        free(tmp);
    }else{
        //assign the string
        // printf("%s", buf);
        errors[lineNo] = (char*)malloc(strlen(buf));
        strcpy(errors[lineNo],buf);
    }
}

void printErrors(int n){
    int i;
    for ( i = 0; i < n; i++) {
        if (errors[i]!= NULL) {
            printf("%s",errors[i] );
        }
    }
}

void whileLoopSemantic(NODE_AstTree stmt, int funcHashVal){
    NODE_AstTree child1 = stmt->child;
    NODE_AstTree child2 = stmt->child->sibling;

    VARLIST vars = NULL;

    vars = extractAllVar(child1,vars);

    int flag = 0;
    VARLIST temp = vars;

    while (temp!=NULL) {

        if (checkUpdate(temp->v,child2)) {
            flag = 1;
            break;
        }
        temp = temp->next;
    }

    if (!flag) {
        // error no one gets updated
        int start = stmt->child->tokens->tk->lineNo;
        int end = start;
        NODE_AstTree tmp = stmt->child->sibling->child;
        while (tmp->sibling != NULL) {
            tmp = tmp->sibling ;
        }
        end = tmp->child->tokens->tk->lineNo+1;
        memset(buf,0,256);
        sprintf(buf,"Line:%d-%d ==> No variable is getting updated in while loop\n",start,end );
        addError(start);
    }

}

VARLIST addVar(NODE_AstTree var, VARLIST ls){
    if (ls==NULL) {
        ls = (VARLIST)malloc(sizeof(varList));
        ls->v = var;
        ls->next = NULL;
        return ls;
    }else{
        VARLIST tmp = ls;
        ls = (VARLIST)malloc(sizeof(varList));
        ls->v = var;
        ls->next = tmp;
        return ls;
    }
}

VARLIST extractAllVar(NODE_AstTree expr, VARLIST vars){
    if (expr->child == NULL) {
        if (expr->tokens->tk->token == TK_ID) {
            vars = addVar(expr,vars);
            return vars;
        }
        return vars;
    }else{
        if(expr->tokens->tk->token == TK_NOT){
            vars = extractAllVar(expr->child,vars);
        }else{
            vars = extractAllVar(expr->child,vars);
            vars = extractAllVar(expr->child->sibling,vars);
        }
        return vars;
    }
}

int checkUpdate(NODE_AstTree var, NODE_AstTree stmts){

    NODE_AstTree stmt = stmts->child;

    while (stmt!=NULL) {

        tokenType tok = stmt->tokens->tk->token;
        switch (tok) {
            case TK_ASSIGNOP:{
                if (!strcmp(var->tokens->tk->lexeme, stmt->child->tokens->tk->lexeme)) {
                    return 1;
                }
            }break;
            case TK_READ:{
                if (!strcmp(var->tokens->tk->lexeme, stmt->child->tokens->tk->lexeme)) {
                    return 1;
                }
            }break;
            case TK_FUNID:{
                NODE_AstTree tmp = stmt->child->child;
                while (tmp!=NULL) {
                    if (!strcmp(var->tokens->tk->lexeme, tmp->tokens->tk->lexeme)) {
                        return 1;
                    }
                    tmp = tmp->sibling;
                }
            }break;
            case TK_WRITE:{

            }break;
            case TK_IF:{
                if (checkUpdate(var,stmt->child->sibling) == 1) {
                    return 1;
                }else if (checkUpdate(var,stmt->child->sibling->sibling) == 1) {
                    return 1;
                }else{
                    return 0;
                }
            }break;
            case TK_WHILE:{
                return checkUpdate(var,stmt->child->sibling);
            }
        }

        stmt = stmt->sibling;
    }

    return 0;
}
