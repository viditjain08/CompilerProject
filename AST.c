#include "astDef.h"

NODE_AstTree buildAST(TREE_NODE root){

    if(root!=NULL){

        if(root->child == NULL){
            NODE_AstTree node = (NODE_AstTree)malloc(sizeof(Node_AstTree));
            node->child = NULL;
            node->sibling = NULL;
            node->parent = NULL;
            node->tokens = (LIST_TokenInfo)malloc(sizeof(List_TokenInfo));
            node->tokens->tk = root->tk_info.tk;
            node->tokens->next = NULL;
            return node;
        }else{


    int rule = root->rule_index + 1;
    TREE_NODE children = root->child;
    switch (rule) {

        case 1:{

            NODE_AstTree child1 = buildAST(children);
            NODE_AstTree child2 = buildAST(children->next);

            NODE_AstTree program = (NODE_AstTree)malloc(sizeof(Node_AstTree));
            program->sibling = NULL;
            program->tokens = NULL;
            program->parent = NULL;

            if(child1->tokens == NULL){ // it does not go to eps
                NODE_AstTree otherFunc = (NODE_AstTree)malloc(sizeof(Node_AstTree));
                otherFunc->tokens = NULL;
                otherFunc->sibling = child2;
                otherFunc->child = child1;
                otherFunc->parent = program;
                child2->sibling = NULL;
                child2->parent = NULL;
                program->child = otherFunc;
            }else{ // it contains EPS info
                // program->child = child1;
                // child1->sibling = child2;
                // child1->parent = program;
                // child2->parent = program;
                // child2->sibling = NULL;
                return child2;
            }
            return program;
        }break;

        case 2:{
            // NODE_AstTree child1 = buildAST(children);
            NODE_AstTree child2 = buildAST(children->next);
            // NODE_AstTree child3 = buildAST(children->next->next);
            child2->sibling = NULL;
            // free(child1);
            // free(child3);
            return child2;
        }break;

        case 3:{
            NODE_AstTree child1 = buildAST(children);
            NODE_AstTree child2 = buildAST(children->next);

            if(child2->tokens == NULL){// it contains Func Info
                child1->sibling = child2;
                return child1;
            }else{ // it contains EPs info
                free(child2);
                child1->sibling = NULL;
                return child1;
            }

        }break;

        case 4:{
            return buildAST(children);
        }break;

        case 5:{
            NODE_AstTree child1 = buildAST(children);
            NODE_AstTree child2 = buildAST(children->next);
            NODE_AstTree child3 = buildAST(children->next->next);
            // NODE_AstTree child4 = buildAST(children->next->next->next);
            NODE_AstTree child5 = buildAST(children->next->next->next->next);
            // NODE_AstTree child6 = buildAST(children->next->next->next->next->next);

            NODE_AstTree res = (NODE_AstTree)malloc(sizeof(Node_AstTree));
            res->tokens = NULL;
            res->parent = NULL;
            res->child = child1;
            res->child->sibling = child2;
            res->child->sibling->sibling = child3;
            res->child->sibling->sibling->sibling = child5;
            // free(child4);
            // free(child6);
            res->sibling = NULL;
            child1->parent = res;
            child2->parent = res;
            child3->parent = res;
            child5->parent = res;
            child5->sibling = NULL;
            return res;
        }break;

        case 6:{
            // NODE_AstTree child1 = buildAST(children);
            // NODE_AstTree child2 = buildAST(children->next);
            // NODE_AstTree child3 = buildAST(children->next->next);
            // NODE_AstTree child4 = buildAST(children->next->next->next);
            NODE_AstTree child5 = buildAST(children->next->next->next->next);
            // NODE_AstTree child6 = buildAST(children->next->next->next->next->next);

            NODE_AstTree res = (NODE_AstTree)malloc(sizeof(Node_AstTree));
            res->tokens = NULL;
            res->parent = NULL;
            res->sibling = NULL;
            res->child = child5;

            return res;

        }break;

        case 7:{
            // NODE_AstTree child1 = buildAST(children);
            // NODE_AstTree child2 = buildAST(children->next);
            // NODE_AstTree child3 = buildAST(children->next->next);
            // NODE_AstTree child4 = buildAST(children->next->next->next);
            NODE_AstTree child5 = buildAST(children->next->next->next->next);
            // NODE_AstTree child6 = buildAST(children->next->next->next->next->next);

            if(child5->tokens->tk->token != EPS){ // it contains parameters info
                NODE_AstTree res = (NODE_AstTree)malloc(sizeof(Node_AstTree));
                res->tokens = NULL;
                res->parent = NULL;
                res->sibling = NULL;
                res->child = child5;

                return res;
            }else{// it contains EPS info
                return child5;
            }


        }break;
        case 8:{
            return buildAST(children);
        }break;
        case 9:{
            NODE_AstTree child1 = buildAST(children);
            NODE_AstTree child2 = buildAST(children->next);
            NODE_AstTree child3 = buildAST(children->next->next);

            if(child1->tokens->tk->token == TK_RECORD){
                child1->tokens->next->next = (LIST_TokenInfo)malloc(sizeof(List_TokenInfo));
                child1->tokens->next->next->tk = child2->tokens->tk;
                free(child2);
            }else{
                child1->tokens->next = (LIST_TokenInfo)malloc(sizeof(List_TokenInfo));
                child1->tokens->next->tk = child2->tokens->tk;
                free(child2);
            }

            if(child3->tokens->tk->token != EPS){ // it contains parameter Info
                child1->sibling = child3;
            }else{  // it contains EPS info
                child1->sibling = NULL;
                free(child3);

            }

            return child1;


        }break;
        case 10:{
            return buildAST(children);
        }break;
        case 11:{
            return buildAST(children);
        }break;
        case 12:{
            return buildAST(children);
        }break;
        case 13:{
            return buildAST(children);
        }break;
        case 14:{
            NODE_AstTree child1 = buildAST(children);
            NODE_AstTree child2 = buildAST(children->next);

            child1->tokens->next = (LIST_TokenInfo)malloc(sizeof(List_TokenInfo));
            child1->tokens->next->tk = child2->tokens->tk;
            child1->sibling = NULL;
            child1->parent = NULL;
            child1->child = NULL;
            free(child2);
            return child1;


        }break;
        case 15:{
            // NODE_AstTree child1 = buildAST(children);
            NODE_AstTree child2 = buildAST(children->next);

            // free(child1);
            return child2;
        }break;

        case 16:{
            return buildAST(children);
        }break;
        case 17:{
            NODE_AstTree child1 = buildAST(children);
            NODE_AstTree child2 = buildAST(children->next);
            NODE_AstTree child3 = buildAST(children->next->next);
            NODE_AstTree child4 = buildAST(children->next->next->next);

            NODE_AstTree temp1;
            if(child1->tokens == NULL){// it contains some typedefs
                temp1 = (NODE_AstTree)malloc(sizeof(Node_AstTree));
                temp1->parent = NULL;
                temp1->tokens = NULL;
                temp1->sibling = NULL;
                temp1->child = child1;
            }else{
                temp1 = child1;
            }

            NODE_AstTree temp2;
            if(child2->tokens->tk->token != EPS){// it contains some declarations
                temp2 = (NODE_AstTree)malloc(sizeof(Node_AstTree));
                temp2->parent = NULL;
                temp2->tokens = NULL;
                temp2->sibling = NULL;
                temp2->child = child2;
            }else{
                temp2 = child2;
            }
            NODE_AstTree temp3;
            if(child3->tokens->tk->token != EPS){// it contains some stmts
                temp3 = (NODE_AstTree)malloc(sizeof(Node_AstTree));
                temp3->parent = NULL;
                temp3->tokens = NULL;
                temp3->sibling = NULL;
                temp3->child = child3;
            }else{
                temp3 = child3;
            }
            NODE_AstTree temp4 = child4;


            NODE_AstTree res = (NODE_AstTree)malloc(sizeof(Node_AstTree));
            res->tokens = NULL;
            res->parent = NULL;
            res->sibling = NULL;
            res->child = temp1;

            temp1->sibling = temp2;
            temp2->sibling = temp3;
            temp3->sibling = temp4;
            temp4->sibling = NULL;

            temp1->parent = res;
            temp2->parent = res;
            temp3->parent = res;
            temp4->parent = res;
            return res;
        }break;
        case 18:{
            NODE_AstTree child1 = buildAST(children);
            NODE_AstTree child2 = buildAST(children->next);

            if(child2->tokens == NULL){// it contains typedef Info
                child1->sibling = child2;
                return child1;
            }else{ // it contains EPs info
                free(child2);
                child1->sibling = NULL;
                return child1;
            }
        }break;
        case 19:{
            return buildAST(children);
        }break;
        case 20:{
            NODE_AstTree child1 = buildAST(children);
            NODE_AstTree child2 = buildAST(children->next);
            NODE_AstTree child3 = buildAST(children->next->next);
            // NODE_AstTree child4 = buildAST(children->next->next->next);
            // NODE_AstTree child5 = buildAST(children->next->next->next->next);

            NODE_AstTree res = (NODE_AstTree)malloc(sizeof(Node_AstTree));
            res->tokens = NULL;
            res->sibling = NULL;
            res->parent = NULL;
            res->child = child1;

            child1->tokens->next = (LIST_TokenInfo)malloc(sizeof(List_TokenInfo));
            child1->tokens->next->tk = child2->tokens->tk;
            child1->sibling = child3;
            free(child2);
            // free(child4);
            // free(child5);
            child1->parent = res;
            child3->parent = res;
            return res;
        }break;
        case 21:{
            NODE_AstTree child1 = buildAST(children);
            NODE_AstTree child2 = buildAST(children->next);
            NODE_AstTree child3 = buildAST(children->next->next);

            NODE_AstTree res = (NODE_AstTree)malloc(sizeof(Node_AstTree));
            res->tokens = NULL;
            res->sibling = NULL;
            res->parent = NULL;
            res->child = child1;

            if(child3->tokens->tk->token !=  EPS){
                child1->parent = res;
                child1->sibling = child2;
                child2->parent = res;
                child2->sibling = child3;
                child3->parent = res;
                child3->sibling = NULL;
            }else{
                child1->sibling = child2;
                child2->sibling = NULL;
                free(child3);
                child1->parent = res;
                child2->parent = res;
            }
            return res;
        }break;
        case 22:{
            // NODE_AstTree child1 = buildAST(children);
            NODE_AstTree child2 = buildAST(children->next);
            // NODE_AstTree child3 = buildAST(children->next->next);
            NODE_AstTree child4 = buildAST(children->next->next->next);
            // NODE_AstTree child5 = buildAST(children->next->next->next->next);


            child2->tokens->next = (LIST_TokenInfo)malloc(sizeof(List_TokenInfo));
            child2->tokens->next->tk = child4->tokens->tk;
            child2->sibling = NULL;
            // free(child1);
            // free(child3);
            free(child4);
            // free(child5);
            return child2;

        }break;
        case 23:{
            NODE_AstTree child1 = buildAST(children);
            NODE_AstTree child2 = buildAST(children->next);

            if(child2->tokens->tk->token != EPS){
                child1->sibling = child2;
            }else{
                child1->sibling =  NULL;
                free(child2);
            }
            return child1;
        }break;
        case 24:{
            return buildAST(children);
        }break;
        case 25:{
            NODE_AstTree child1 = buildAST(children);
            NODE_AstTree child2 = buildAST(children->next);

            if(child2->tokens->tk->token != EPS){// it contains declarations Info
                child1->sibling = child2;
                return child1;
            }else{ // it contains EPs info
                free(child2);
                child1->sibling = NULL;
                return child1;
            }

        }break;
        case 26:{
            return buildAST(children);
        }break;
        case 27:{
            // NODE_AstTree child1 = buildAST(children);
            NODE_AstTree child2 = buildAST(children->next);
            // NODE_AstTree child3 = buildAST(children->next->next);
            NODE_AstTree child4 = buildAST(children->next->next->next);
            NODE_AstTree child5 = buildAST(children->next->next->next->next);
            // NODE_AstTree child6 = buildAST(children->next->next->next->next->next);

            child2->tokens->next = (LIST_TokenInfo)malloc(sizeof(List_TokenInfo));
            child2->tokens->next->tk = child4->tokens->tk;
            child2->tokens->next->next = (LIST_TokenInfo)malloc(sizeof(List_TokenInfo));
            child2->tokens->next->next->tk = child5->tokens->tk;
            // free(child1);
            // free(child3);
            free(child4);
            free(child5);
            // free(child6);
            return child2;
        }break;
        case 28:{
            // NODE_AstTree child1 = buildAST(children);;
            NODE_AstTree child2 = buildAST(children->next);;
            // free(child1);
            child2->sibling = NULL;
            return child2;
        }break;
        case 29:{
            return buildAST(children);
        }break;
        case 30:{
            NODE_AstTree child1 = buildAST(children);
            NODE_AstTree child2 = buildAST(children->next);

            if(child2->tokens->tk->token != EPS){// it contains Func Info
                child1->sibling = child2;
                return child1;
            }else{ // it contains EPs info
                free(child2);
                child1->sibling = NULL;
                return child1;
            }

        }break;
        case 31:{
            return buildAST(children);
        }break;
        case 32:{
            NODE_AstTree child1 = buildAST(children);
            child1->tokens = (LIST_TokenInfo)malloc(sizeof(List_TokenInfo));
            child1->tokens->tk = (TOKENINFO)malloc(sizeof(tokenInfo));
            child1->tokens->next = NULL;
            child1->tokens->tk->token = TK_ASSIGNOP;
            child1->tokens->tk->lexeme = "<---";
            child1->tokens->tk->lineNo = 0;
            child1->tokens->tk->val = NULL;
            child1->tokens->tk->dataType = 4;
            return child1;

        }break;
        case 33:{
            NODE_AstTree child1 = buildAST(children);
            child1->tokens = (LIST_TokenInfo)malloc(sizeof(List_TokenInfo));
            child1->tokens->tk = (TOKENINFO)malloc(sizeof(tokenInfo));
            child1->tokens->next = NULL;
            child1->tokens->tk->token = TK_WHILE;
            child1->tokens->tk->lexeme = "while";
            child1->tokens->tk->lineNo = 0;
            child1->tokens->tk->val = NULL;
            child1->tokens->tk->dataType = 4;
            return child1;
        }break;
        case 34:{
            NODE_AstTree child1 = buildAST(children);
            child1->tokens = (LIST_TokenInfo)malloc(sizeof(List_TokenInfo));
            child1->tokens->tk = (TOKENINFO)malloc(sizeof(tokenInfo));
            child1->tokens->next = NULL;
            child1->tokens->tk->token = TK_IF;
            child1->tokens->tk->lexeme = "if";
            child1->tokens->tk->lineNo = 0;
            child1->tokens->tk->val = NULL;
            child1->tokens->tk->dataType = 4;
            return child1;
        }break;
        case 35:{
            NODE_AstTree child1 = buildAST(children);
            return child1;
        }break;
        case 36:{
            NODE_AstTree child1 = buildAST(children);
            return child1;
        }break;

        case 37:{
            NODE_AstTree child1 = buildAST(children);
            // NODE_AstTree child2 = buildAST(children->next);
            NODE_AstTree child3 = buildAST(children->next->next);
            // NODE_AstTree child4 = buildAST(children->next->next->next);

            NODE_AstTree res = (NODE_AstTree)malloc(sizeof(Node_AstTree));
            res->tokens = NULL;
            res->sibling = NULL;
            res->parent = NULL;
            res->child = child1;

            child1->sibling = child3;
            child3->sibling = NULL;
            child1->parent = res;
            child3->parent = res;

            // free(child2); free(child4);
            return res;

        }break;
        case 38:{
            NODE_AstTree child1 = buildAST(children);
            NODE_AstTree child2 = buildAST(children->next);

            child1->sibling = NULL;
            if(child2->tokens->tk->token == EPS){
                free(child2);
                return child1;
            }else{
                child1->tokens->next = (LIST_TokenInfo)malloc(sizeof(List_TokenInfo));
                child1->tokens->next->tk = child2->tokens->tk;
                free(child2);
                return child1;
            }

        }break;
        case 39:{
            // NODE_AstTree child1 = children;
            NODE_AstTree child2 = buildAST(children->next);
            // free(child1);
            return child2;
        }break;
        case 40:{
            return buildAST(children);
        }break;
        case 41:{
            NODE_AstTree child1 = buildAST(children);
            // NODE_AstTree child2 = buildAST(children->next);
            NODE_AstTree child3 = buildAST(children->next->next);
            // NODE_AstTree child4 = buildAST(children->next->next->next);
            // NODE_AstTree child5 = buildAST(children->next->next->next->next);
            NODE_AstTree child6 = buildAST(children->next->next->next->next->next);
            // NODE_AstTree child7 = buildAST(children->next->next->next->next->next->next);

            child3->child = child1;
            child1->sibling = child6;
            child6->sibling = NULL;
            child1->parent = child3;
            child6->parent = child3;
            child3->sibling = NULL;
            return child3;

        }break;
        case 42:{
            // NODE_AstTree child1 = buildAST(children);
            NODE_AstTree child2 = buildAST(children->next);
            // NODE_AstTree child3 = buildAST(children->next->next);
            // NODE_AstTree child4 = buildAST(children->next->next->next);

            NODE_AstTree res = (NODE_AstTree)malloc(sizeof(Node_AstTree));
            res->tokens = NULL;
            res->sibling = NULL;
            res->parent = NULL;
            res->child = child2;
            return res;

        }break;
        case 43:{
            return buildAST(children);
        }break;
        case 44:{
            // NODE_AstTree child1 = buildAST(children);
            NODE_AstTree child2 = buildAST(children->next);
            // NODE_AstTree child3 = buildAST(children->next->next);

            NODE_AstTree res = (NODE_AstTree)malloc(sizeof(Node_AstTree));
            res->tokens = NULL;
            res->sibling = NULL;
            res->parent = NULL;
            res->child = child2;
            return res;
        }break;
        case 45:{
            // NODE_AstTree child1 = buildAST(children);
            // NODE_AstTree child2 = buildAST(children->next);
            NODE_AstTree child3 = buildAST(children->next->next);
            // NODE_AstTree child4 = buildAST(children->next->next->next);
            NODE_AstTree child5 = buildAST(children->next->next->next->next);
            NODE_AstTree child6 = buildAST(children->next->next->next->next->next);
            // NODE_AstTree child7 = buildAST(children->next->next->next->next->next->next);

            if(child6->tokens->tk->token != EPS){// it contains Func Info
                child5->sibling = child6;
            }else{ // it contains EPs info
                free(child6);
                child5->sibling = NULL;
            }

            NODE_AstTree tmp = (NODE_AstTree)malloc(sizeof(Node_AstTree));
            tmp->tokens = NULL;
            tmp->sibling = NULL;
            tmp->parent = NULL;
            tmp->child = child5;

            NODE_AstTree res = (NODE_AstTree)malloc(sizeof(Node_AstTree));
            res->tokens = NULL;
            res->sibling = NULL;
            res->parent = NULL;
            res->child = child3;

            child3->sibling = tmp;
            child3->parent = res;
            tmp->parent = res;

            return res;



        }break;
        case 46:{
            // 46. <conditionalStmt>===> TK_IF TK_OP <booleanExpression> TK_CL TK_THEN <stmt> <otherStmts> <elsePart>
            // NODE_AstTree child1 = buildAST(children);
            // NODE_AstTree child2 = buildAST(children->next);
            NODE_AstTree child3 = buildAST(children->next->next);
            // NODE_AstTree child4 = buildAST(children->next->next->next);
            // NODE_AstTree child5 = buildAST(children->next->next->next->next);
            NODE_AstTree child6 = buildAST(children->next->next->next->next->next);
            NODE_AstTree child7 = buildAST(children->next->next->next->next->next->next);
            NODE_AstTree child8 = buildAST(children->next->next->next->next->next->next->next);

            if(child7->tokens->tk->token != EPS){// it contains Func Info
                child6->sibling = child7;
            }else{ // it contains EPs info
                free(child7);
                child6->sibling = NULL;
            }

            NODE_AstTree tmp = (NODE_AstTree)malloc(sizeof(Node_AstTree));
            tmp->tokens = NULL;
            tmp->sibling = NULL;
            tmp->parent = NULL;
            tmp->child = child6;

            child6->parent = tmp;

            child3->sibling = tmp;
            tmp->sibling = child8;
            child8->sibling = NULL;

            NODE_AstTree res = (NODE_AstTree)malloc(sizeof(Node_AstTree));
            res->tokens = NULL;
            res->sibling = NULL;
            res->parent = NULL;
            res->child = child3;

            child3->parent = res;
            tmp->parent = res;
            child8->parent = res;

            return res;

        }break;
        case 47:{
            //47. <elsePart>===> TK_ELSE <stmt> <otherStmts> TK_ENDIF
            // NODE_AstTree child1 = buildAST(children);
            NODE_AstTree child2 = buildAST(children->next);
            NODE_AstTree child3 = buildAST(children->next->next);
            // NODE_AstTree child4 = buildAST(children->next->next->next);
            if(child3->tokens->tk->token != EPS){// it contains Func Info
                child2->sibling = child3;
            }else{ // it contains EPs info
                free(child3);
                child2->sibling = NULL;
            }

            NODE_AstTree res = (NODE_AstTree)malloc(sizeof(Node_AstTree));
            res->tokens = NULL;
            res->sibling = NULL;
            res->parent = NULL;
            res->child = child2;

            child2->parent = res;
            return res;


        }break;
        case 48:{
            return buildAST(children);
        }break;
        case 49:{
            NODE_AstTree child1 = buildAST(children);
            // NODE_AstTree child2 = buildAST(children->next);
            NODE_AstTree child3 = buildAST(children->next->next);
            // NODE_AstTree child4 = buildAST(children->next->next->next);
            // NODE_AstTree child5 = buildAST(children->next->next->next->next);


            child1->sibling = NULL;
            child1->child = child3;
            child3->parent = child1;
            child3->sibling = NULL;

            // free(child2); free(child4); free(child5);

            return child1;

        }break;
        case 50:{
            NODE_AstTree child1 = buildAST(children);
            // NODE_AstTree child2 = buildAST(children->next);
            NODE_AstTree child3 = buildAST(children->next->next);
            // NODE_AstTree child4 = buildAST(children->next->next->next);
            // NODE_AstTree child5 = buildAST(children->next->next->next->next);


            child1->sibling = NULL;
            child1->child = child3;
            child3->parent = child1;
            child3->sibling = NULL;

            // free(child2); free(child4); free(child5);

            return child1;
        }break;

        case 51:{
            NODE_AstTree child1 = buildAST(children);
            NODE_AstTree child2 = buildAST(children->next);

            if (child2->tokens->tk->token == EPS) { // expPrime contains EPS info
                free(child2);
                return child1;
            }else{
                NODE_AstTree tmp = child2->child;
                child2->child = child1;
                child1->parent = child2;
                child1->sibling = tmp;

                tmp = child2;
                while(tmp->parent != NULL){
                    tmp = tmp->parent;
                }
                return tmp;
            }

        }break;
        case 52:{
            NODE_AstTree child1 = buildAST(children);
            NODE_AstTree child2 = buildAST(children->next);
            NODE_AstTree child3 = buildAST(children->next->next);

            if (child3->tokens->tk->token == EPS) { // expPrime contains EPS info
                free(child3);
                child1->child = child2;
                child2->parent = child1;
                child2->sibling = NULL;
                child1->sibling = NULL;
                return child1;
            }else{
                NODE_AstTree tmp = child3->child;
                child3->child = child1;
                child1->child = child2;
                child1->parent = child3;
                child2->parent = child1;
                child1->sibling = tmp;
                return child1;
            }

        }break;
        case 53:{
            return buildAST(children);
        }break;
        case 54:{
            NODE_AstTree child1 = buildAST(children);
            NODE_AstTree child2 = buildAST(children->next);

            if (child2->tokens->tk->token == EPS) { // expPrime contains EPS info
                free(child2);
                return child1;
            }else{
                NODE_AstTree tmp = child2->child;
                child2->child = child1;
                child1->parent = child2;
                child1->sibling = tmp;

                tmp = child2;
                while(tmp->parent != NULL){
                    tmp = tmp->parent;
                }
                return tmp;
            }

        }break;
        case 55:{
            NODE_AstTree child1 = buildAST(children);
            NODE_AstTree child2 = buildAST(children->next);
            NODE_AstTree child3 = buildAST(children->next->next);

            if (child3->tokens->tk->token == EPS) { // expPrime contains EPS info
                free(child3);
                child1->child = child2;
                child2->parent = child1;
                child2->sibling = NULL;
                child1->sibling = NULL;
                return child1;
            }else{
                NODE_AstTree tmp = child3->child;
                child3->child = child1;
                child1->child = child2;
                child1->parent = child3;
                child2->parent = child1;
                child1->sibling = tmp;
                return child1;
            }
        }break;
        case 56:{
            return buildAST(children);
        }break;
        case 57:{
            // NODE_AstTree child1 = buildAST(children);
            NODE_AstTree child2 = buildAST(children->next);
            // NODE_AstTree child3 = buildAST(children->next);
            return child2;
        }break;
        case 58:{
            return buildAST(children);
        }break;
        case 59:{
            return buildAST(children);
        }break;
        case 60:{
            return buildAST(children);
        }break;
        case 61:{
            return buildAST(children);
        }break;
        case 62:{
            return buildAST(children);
        }break;
        case 63:{
            return buildAST(children);
        }break;
        case 64:{
            return buildAST(children);
        }break;
        case 65:{
            return buildAST(children);
        }break;
        case 66:{
            // NODE_AstTree child1 = buildAST(children);
            NODE_AstTree child2 = buildAST(children->next);
            // NODE_AstTree child3 = buildAST(children->next->next);
            NODE_AstTree child4 = buildAST(children->next->next->next);
            // NODE_AstTree child5 = buildAST(children->next->next->next->next);
            NODE_AstTree child6 = buildAST(children->next->next->next->next->next);
            // NODE_AstTree child7 = buildAST(children->next->next->next->next->next->next);

            child2->sibling = child6;
            child6->sibling = NULL;
            child2->parent = child4;
            child6->parent = child4;
            child4->child  = child2;
            child2->sibling = NULL;
            return child4;
        }break;
        case 67:{
            NODE_AstTree child1 = buildAST(children);
            NODE_AstTree child2 = buildAST(children->next);
            NODE_AstTree child3 = buildAST(children->next->next);

            child1->sibling = child3;
            child3->sibling = NULL;
            child1->parent = child2;
            child3->parent = child2;
            child2->child = child1;
            child2->sibling = NULL;

            return child2;
        }break;
        case 68:{
            NODE_AstTree child1 = buildAST(children);
            // NODE_AstTree child2 = buildAST(children->next);
            NODE_AstTree child3 = buildAST(children->next->next);
            // NODE_AstTree child4 = buildAST(children->next->next->next);
            child1->child = child3;
            child1->sibling = NULL;
            child3->sibling = NULL;
            child3->parent = child1;

            return child1;
        }break;
        case 69:{
            return buildAST(children);
        }break;
        case 70:{
            return buildAST(children);
        }break;
        case 71:{
            return buildAST(children);
        }break;
        case 72:{
            return buildAST(children);
        }break;
        case 73:{
            return buildAST(children);
        }break;
        case 74:{
            return buildAST(children);
        }break;
        case 75:{
            return buildAST(children);
        }break;
        case 76:{
            return buildAST(children);
        }break;
        case 77:{
            // NODE_AstTree child1 = buildAST(children);
            NODE_AstTree child2 = buildAST(children->next);
            // NODE_AstTree child3 = buildAST(children->next->next);
            return child2;
        }break;
        case 78:{
            // NODE_AstTree child1 = buildAST(children);
            NODE_AstTree child2 = buildAST(children->next);
            // NODE_AstTree child3 = buildAST(children->next->next);
            NODE_AstTree res = (NODE_AstTree)malloc(sizeof(Node_AstTree));
            res->tokens = NULL;
            res->sibling = NULL;
            res->parent = NULL;
            res->child = child2;
            return res;


        }break;
        case 79:{
            return buildAST(children);
        }break;
        case 80:{
            NODE_AstTree child1 = buildAST(children);
            NODE_AstTree child2 = buildAST(children->next);
            if(child2->tokens->tk->token != EPS){
                child1->sibling = child2;
                return child1;
            }else{
                child1->sibling = NULL;
                free(child2);
                return child1;
            }
        }break;
        case 81:{
            // NODE_AstTree child1 = buildAST(children);
            NODE_AstTree child2 = buildAST(children->next);
            // free(child1);
            // child2->sibling = NULL;
            return child2;
        }break;
        case 82:{
            return buildAST(children);
        }break;

    }








        }
    }



}

int countNodesAST(NODE_AstTree root){
    NODE_AstTree temp = root->child;
    int sum = 0;
    int t;
    while(temp != NULL){
        t = countNodesAST(temp);
        sum += t;
        temp = temp->sibling;
    }

    return sum +1;
}
int countNodesParseTree(TREE_NODE root){
    TREE_NODE temp = root->child;
    int sum = 0;

    while(temp != NULL){
        sum += countNodesParseTree(temp);
        temp = temp->next;
    }

    return sum +1;
}
