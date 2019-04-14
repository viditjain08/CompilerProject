#include "typeChecker.h"

dataType getIdentifierDtype(NODE_AstTree var, int funcHashVal){
    int index ;
    if(var->tokens->tk->token == TK_NUM){
        return INT;
    }else if(var->tokens->tk->token == TK_RNUM){
        return REAL;
    }
    if(var->tokens->next == NULL){// single identifier is there
        index = lookupEntry("global",var->tokens->tk->lexeme,NULL);
        if(index < 0){
            // error, var not defined
            index = lookupEntry(functions[funcHashVal].function_name,var->tokens->tk->lexeme,NULL);
            if(index < 0){
                // error, var not defined
                printf("Line:%d ==> %s variable not defined\n",var->tokens->tk->lineNo,var->tokens->tk->lexeme );
                return ERROR;
            }
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
        index = lookupEntry("global",var->tokens->tk->lexeme,var->tokens->next->tk->lexeme);
        if(index < 0){
            // error, var not defined
            index = lookupEntry(functions[funcHashVal].function_name,var->tokens->tk->lexeme,var->tokens->next->tk->lexeme);
            if(index < 0){
                // error, var not defined
                printf("Line:%d ==> %s variable not defined\n",var->tokens->tk->lineNo,var->tokens->tk->lexeme);
                return ERROR;
            }
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
                        printf("Line:%d ==> Mul/Div Operation is not valid records %s and %s \n",child1->tokens->tk->lineNo,child1->tokens->tk->lexeme,child2->tokens->tk->lexeme);
                        return ERROR;
                    }else{
                        return dtChild1;
                    }
                }else{
                    printf("Line:%d ==> Data type of %s and %s don't match\n", child1->tokens->tk->lineNo,child1->tokens->tk->lexeme,child2->tokens->tk->lexeme);
                    return ERROR;
                }
            }
            break;

            case TK_NOT:{

                NODE_AstTree child1 = root->child;
                dataType dt = getExpressionDtype(child1,funcHashVal);
                if (dt != BOOL) {
                    printf("Line:%d ==> Operation is not valid on %s\n",child1->tokens->tk->lineNo,child1->tokens->tk->lexeme );
                }
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
                        printf("Line:%d ==> Operation is not valid between %s and %s\n", child1->tokens->tk->lineNo,child1->tokens->tk->lexeme,child2->tokens->tk->lexeme);
                        return ERROR;
                    }
                }else{
                    printf("Line:%d ==> Data type of %s and %s don't match\n", child1->tokens->tk->lineNo,child1->tokens->tk->lexeme,child2->tokens->tk->lexeme);
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
                        return BOOL;
                    }else{
                        printf("Line:%d ==> Operation is not valid between %s and %s\n", child1->tokens->tk->lineNo,child1->tokens->tk->lexeme,child2->tokens->tk->lexeme);
                        return ERROR;
                    }
                }else{
                    printf("Line:%d ==> Data type of %s and %s don't match\n", child1->tokens->tk->lineNo,child1->tokens->tk->lexeme,child2->tokens->tk->lexeme);
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
    while (tmp1->child != NULL) {
        tmp1 = tmp1->child;
    }
    NODE_AstTree tmp2 = child2;
    while (tmp2->child != NULL) {
        tmp2 = tmp2->child;
    }
    int ind1 = lookupEntry("global",tmp1->tokens->tk->lexeme,NULL);
    if(ind1 < 0){
        ind1 = lookupEntry(functions[funcHashVal].function_name,tmp1->tokens->tk->lexeme,NULL);
        if(ind1 < 0){
            // error, var not defined
            // printf("Line:%d ==> %s variable is not defined\n",child1->tokens->tk->lineNo,child1->tokens->tk->lexeme );
            return ERROR;
        }
    }

    int ind2 = lookupEntry("global",tmp2->tokens->tk->lexeme,NULL);
    if(ind2 < 0){
        // error, var not defined
        ind1 = lookupEntry(functions[funcHashVal].function_name,tmp2->tokens->tk->lexeme,NULL);
        if(ind2 < 0){
            // error, var not defined
            // printf("Line:%d ==> %s variable is not defined\n",child2->tokens->tk->lineNo,child2->tokens->tk->lexeme );
            return ERROR;
        }
    }
    hashsymbol h1 = h[ind1];
    hashsymbol h2 = h[ind2];


    if (strcmp(h1.entry_ptr->record_name,h2.entry_ptr->record_name)==0) {
        return RECORD;
    }else{
        printf("Line:%d ==> %s and %s record types don't match\n",child1->tokens->tk->lineNo,child1->tokens->tk->lexeme,child2->tokens->tk->lexeme );
        return ERROR;
    }

}
