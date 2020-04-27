/*
    Group 1:
    1. Anupam Aggarwal     2016A7PS0033P
    2. Piyush Garg         2016A7PS0035P
    3. Rijul Katiyar       2016A7PS0063P
    4. Vidit Jain          2016A7PS0064P

*/
#include "typeChecker.h"
int errno = 0;

int getHashIndex(NODE_AstTree var, int funcHashVal){
    int index;
    if(var->tokens->next == NULL){// single identifier is there

        index = lookupEntry(functions[funcHashVal].function_name,var->tokens->tk->lexeme,NULL);
        if(index < 0){
            // error, var not defined
        	index = lookupEntry("global",var->tokens->tk->lexeme,NULL);
            return index;
        }

    }else{// identifier with field id is there

        index = lookupEntry(functions[funcHashVal].function_name,var->tokens->tk->lexeme,var->tokens->next->tk->lexeme);
        if(index < 0){
            // error, var not defined
	        index = lookupEntry("global",var->tokens->tk->lexeme,var->tokens->next->tk->lexeme);
            return index;
        }

    }
    return index;
}


char *getIdentifierDtype(NODE_AstTree var, int funcHashVal){
    int index ;
    char buf[256];
    int lineNo;
    if(var->tokens->tk->token == TK_NUM){
        return "int";
    }else if(var->tokens->tk->token == TK_RNUM){
        return "real";
    }
    if(var->tokens->next == NULL){// single identifier is there
        index = getHashIndex(var,funcHashVal);
        if (index < 0) {
            errno = 1;
            memset(buf,0,256);
            sprintf(buf,"Line:%d ==> %s variable not declared\n",var->tokens->tk->lineNo,var->tokens->tk->lexeme );
            lineNo = var->tokens->tk->lineNo;
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
            return "error";
        }

        hashsymbol hVar = h[index];

        if(hVar.entry_ptr->int_no == 1 && hVar.entry_ptr->real_no == 0){ //it is an integer
            return "int";
        }else if(hVar.entry_ptr->int_no == 0 && hVar.entry_ptr->real_no == 1){ // it is a real No
            return "real";
        }else if ((hVar.entry_ptr->int_no + hVar.entry_ptr->real_no) > 1){ // it is a record type
            return hVar.entry_ptr->record_name;
        }else{
            return "error";
        }
    }else{// identifier with field id is there
        index = getHashIndex(var,funcHashVal);
        if (index < 0) {
            // errno = 1;
            // printf("Line:%d ==> %s variable not declared\n",var->tokens->tk->lineNo,var->tokens->tk->lexeme );

            errno = 1;
            memset(buf,0,256);
            sprintf(buf,"Line:%d ==> %s variable not declared\n",var->tokens->tk->lineNo,var->tokens->tk->lexeme );
            lineNo = var->tokens->tk->lineNo;
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
            return "error";
        }
        // index = lookupEntry("global",var->tokens->tk->lexeme,var->tokens->next->tk->lexeme);

        hashsymbol hVar = h[index];

        return (hVar.field_ptr->dType==INT) ? "int" : "real";
    }
}

char* getExpressionDtype(NODE_AstTree root, int funcHashVal){
    if (root->child == NULL) { // it is an identifier, a leafNode
        return getIdentifierDtype(root,funcHashVal);
    }else{  // it has 2 children, and is itself a operator node
        tokenType op = root->tokens->tk->token;
        switch (op) {
            case TK_PLUS:
            case TK_MINUS:{
                NODE_AstTree child1 = root->child;
                NODE_AstTree child2 = root->child->sibling;

                char* dtChild1 = getExpressionDtype(child1,funcHashVal);
                char* dtChild2 = getExpressionDtype(child2,funcHashVal);

                if(!strcmp(dtChild1,dtChild2)){
                    return dtChild1;
                }else{

                    return "error";
                }
            }
            break;

            case TK_MUL:
            case TK_DIV:{
                NODE_AstTree child1 = root->child;
                NODE_AstTree child2 = root->child->sibling;

                char* dtChild1 = getExpressionDtype(child1,funcHashVal);
                char* dtChild2 = getExpressionDtype(child2,funcHashVal);

                if (!strcmp(dtChild1,dtChild2)) {
                    if(!strcmp(dtChild1,dtChild2)) {
                    	if(!strcmp(dtChild1,"int")||!strcmp(dtChild1,"real")) {
                    		return dtChild1;
                    	} else {
                    		return "error";
                    	}
                    }
                }else if(!strcmp(dtChild1,"int") && ((strcmp("real",dtChild2) && strcmp("error",dtChild2)))){
                    return dtChild2;
                }else if(!strcmp(dtChild1,"real") && ((strcmp("int",dtChild2) && strcmp("error",dtChild2)))){
                    return dtChild2;
                }else if(!strcmp(dtChild2,"int") && ((strcmp("real",dtChild1) && strcmp("error",dtChild1)))){
                    return dtChild1;
                }else if(!strcmp(dtChild2,"real") && ((strcmp("int",dtChild1) && strcmp("error",dtChild1)))){
                    return dtChild1;
                }else{
                    return "error";
                }
            }
            break;

            case TK_NOT:{

                NODE_AstTree child1 = root->child;
                char* dt = getExpressionDtype(child1,funcHashVal);
                if (!strcmp(dt,"bool")) {
                    // printf("Line:%d ==> Operation is not valid on %s\n",child1->tokens->tk->lineNo,child1->tokens->tk->lexeme );
                    return "bool";
                }
                return "error";
            }
            break;

            case TK_AND:
            case TK_OR:{
                NODE_AstTree child1 = root->child;
                NODE_AstTree child2 = root->child->sibling;

                char* dtChild1 = getExpressionDtype(child1,funcHashVal);
                char* dtChild2 = getExpressionDtype(child2,funcHashVal);
                if(!strcmp(dtChild1,dtChild2)){
                    if(!strcmp(dtChild1,"bool")){
                        return "bool";
                    }else{
                        // printf("Line:%d ==> Operation is not valid between %s and %s\n", child1->tokens->tk->lineNo,child1->tokens->tk->lexeme,child2->tokens->tk->lexeme);

                        return "error";
                    }
                }else{
                    // printf("Line:%d ==> Data type of %s and %s don't match\n", child1->tokens->tk->lineNo,child1->tokens->tk->lexeme,child2->tokens->tk->lexeme);

                    return "error";
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

                char* dtChild1 = getExpressionDtype(child1,funcHashVal);
                char* dtChild2 = getExpressionDtype(child2,funcHashVal);

                if(!strcmp(dtChild1,dtChild2)){
                    if(!strcmp(dtChild1,"int") || !strcmp(dtChild1,"real")){
                        return "bool";
                    }else{
                        // printf("Line:%d ==> Operation is not valid between %s and %s\n", child1->tokens->tk->lineNo,child1->tokens->tk->lexeme,child2->tokens->tk->lexeme);

                        return "error";
                    }
                }else{
                    // printf("Line:%d ==> Data type of %s and %s don't match\n", child1->tokens->tk->lineNo,child1->tokens->tk->lexeme,child2->tokens->tk->lexeme);

                    return "error";
                }
            }
            break;
        }
    }
}
